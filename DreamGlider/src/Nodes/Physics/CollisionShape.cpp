#include "CollisionShape.h"


CollisionShape::CollisionShape(std::string name) : Node3D(name)
{
    type = NODE_TYPE_COLLISION_SHAPE;
    sceneManager->registerCollisionNode(this);
}

CollisionShape::~CollisionShape()
{
    //dtor
}

glm::vec3 closestPointOnLine(glm::vec3 A, glm::vec3 B, glm::vec3 Point)
{
    using namespace glm;
    vec3 AB = B - A;
    float t = dot(Point - A, AB) / dot(AB, AB);
    return A + std::min(std::max(t, 0.0f), 1.0f) * AB;
}

collisionInfo CollisionShape::sphereSphere(CollisionShape* collider)
{
    using namespace glm;
    vec3 myPos(getGlobalPosition());

    float colliderRadius = collider->getRadius();
    vec3 colliderPos(collider->getGlobalPosition());

    float collisionThreshold = radius + colliderRadius;

    vec3 penetrationVector = myPos - colliderPos;
    float dist = length(penetrationVector);

    collisionInfo col;
    col.collided = dist < collisionThreshold;
    if (!col.collided)
        return col;
    vec3 N = normalize(penetrationVector);
    col.collisionNormal = vec4(N,0.0f);
    col.collisionPoint = vec4(myPos -N * radius, 1.0f);
    col.penetrationDepth = collisionThreshold - dist;
    return col;
}

collisionInfo CollisionShape::sphereTriangle(CollisionShape* collider)//From https://wickedengine.net/2020/04/26/capsule-collision-detection/
{
    using namespace glm;
    //std::cout << "Colt tr\n";
    CollisionShape* sphere = this;
    CollisionShape* triangle = collider;

    vec3 center(sphere->getGlobalPosition());

    Mesh3D* collisionMesh = triangle->getMesh();
    int numTriangles = collisionMesh->triangles.size()/3;
    for (int i = 0; i < numTriangles; i++)
    {
        collisionInfo col;
        col.collided = false;
        col.collisionNormal = vec4(0.0f);
        col.collisionPoint = vec4(0.0f);
        col.penetrationDepth = 0.0f;

        int base = i * 3;
        int index0 = collisionMesh->triangles[base];
        int index1 = collisionMesh->triangles[base + 1];
        int index2 = collisionMesh->triangles[base + 2];


        vec3 p0(collider->getGlobalTransform() * collisionMesh->vertices[index0]);
        vec3 p1(collider->getGlobalTransform() * collisionMesh->vertices[index1]);
        vec3 p2(collider->getGlobalTransform() * collisionMesh->vertices[index2]);

        vec3 N = normalize(cross(p1 - p0, p2 - p0));

        float dist = dot(center - p0, N);

        float rad = sphere->radius;

        if (dist < -rad || dist > rad)
            continue;

        vec3 point0 = center - N * dist;

        vec3 c0 = cross(point0 - p0, p1 - p0);
        vec3 c1 = cross(point0 - p1, p2 - p1);
        vec3 c2 = cross(point0 - p2, p0 - p2);

        bool inside = dot(c0, N) <= 0 && dot(c1, N) <= 0 && dot(c2, N) <= 0;

        float rad2 = rad * rad;

        vec3 point1 = closestPointOnLine(p0, p1, center);
        vec3 v1 = center - point1;
        float distSq1 = dot(v1, v1);
        bool intersects = distSq1 < rad2;

        vec3 point2 = closestPointOnLine(p1, p2, center);
        vec3 v2 = center - point2;
        float distSq2 = dot(v2, v2);
        intersects |= distSq2 < rad2;

        vec3 point3 = closestPointOnLine(p2, p0, center);
        vec3 v3 = center - point3;
        float distSq3 = dot(v3, v3);
        intersects |= distSq3 < rad2;

        if (inside || intersects)
        {
            vec3 bestPoint = point0;
            vec3 intersectionVec;

            if (inside)
            {
                intersectionVec = center - point0;
            }
            else
            {
                vec3 d = center - point1;
                float bestDistSq = dot(d, d);
                bestPoint = point1;
                intersectionVec = d;

                d = center - point2;
                float distSq = dot(d,d);
                if (distSq < bestDistSq)
                {
                    bestDistSq = distSq;
                    bestPoint = point2;
                    intersectionVec = d;
                }

                d = center - point3;
                distSq = dot(d,d);
                if (distSq < bestDistSq)
                {
                    bestDistSq = distSq;
                    bestPoint = point3;
                    intersectionVec = d;
                }
            }
            float len = length(intersectionVec);
            col.collided = true;
            col.collisionPoint = vec4(bestPoint, 1.0f);
            col.collisionNormal = vec4(intersectionVec / len, 0.0f);
            col.penetrationDepth = rad - len;
            return col;
        }
    }
    collisionInfo col;
    col.collided = false;
    col.collisionNormal = vec4(0.0f);
    col.collisionPoint = vec4(0.0f);
    col.penetrationDepth = 0.0f;
    return col;
}

collisionInfo CollisionShape::capsuleTriangle(CollisionShape* collider)
{
    using namespace glm;

    CollisionShape* sphere = this;
    CollisionShape* triangle = collider;
    vec3 center(sphere->getGlobalPosition());

    Mesh3D* collisionMesh = triangle->getMesh();
    int numTriangles = collisionMesh->triangles.size()/3;
    for (int i = 0; i < numTriangles; i++)
    {
        collisionInfo col;
        col.collided = false;
        col.collisionNormal = vec4(0.0f);
        col.collisionPoint = vec4(0.0f);
        col.penetrationDepth = 0.0f;

        int baseIndex = i * 3;
        int index0 = collisionMesh->triangles[baseIndex];
        int index1 = collisionMesh->triangles[baseIndex + 1];
        int index2 = collisionMesh->triangles[baseIndex + 2];


        vec3 p0(collider->getGlobalTransform() * collisionMesh->vertices[index0]);
        vec3 p1(collider->getGlobalTransform() * collisionMesh->vertices[index1]);
        vec3 p2(collider->getGlobalTransform() * collisionMesh->vertices[index2]);

        vec3 N = normalize(cross(p1 - p0, p2 - p0));

        //////////////Triangle ray
        vec3 base(getGlobalTransform() * vec4(0.0f,0.0f,0.0f,1.0f));
        vec3 tip(getGlobalTransform() * vec4(0.0f,height,0.0f,1.0f));

        vec3 capsuleNormal = normalize(tip - base);
        vec3 referencePoint;

        vec3 lineEndOffset = capsuleNormal * radius;
        vec3 A = base + lineEndOffset;
        vec3 B = tip - lineEndOffset;

        if (!dot(capsuleNormal, N) == 0.0f)
        {
            referencePoint = p0;
        }
        else
        {

            float t = dot(N, (p0 - base) / abs(dot(N, capsuleNormal)));
            vec3 linePlaneIntersection = base + capsuleNormal * t;

            vec3 cc0 = cross(linePlaneIntersection - p0, p1 - p0);
            vec3 cc1 = cross(linePlaneIntersection - p1, p2 - p1);
            vec3 cc2 = cross(linePlaneIntersection - p2, p0 - p2);

            bool cinside = dot(cc0, N) <= 0 && dot(cc1, N) <= 0 && dot(cc2, N) <= 0;

            if (cinside)
            {
                referencePoint = linePlaneIntersection;
            }
            else
            {
                vec3 point1 = closestPointOnLine(p0, p1, linePlaneIntersection);
                vec3 v1 = linePlaneIntersection - point1;
                float distSq = dot(v1,v1);
                float bestDist = distSq;
                referencePoint = point1;

                vec3 point2 = closestPointOnLine(p1, p2, linePlaneIntersection);
                vec3 v2 = linePlaneIntersection - point2;
                distSq = dot(v2,v2);
                if (distSq < bestDist)
                {
                    referencePoint = point2;
                    bestDist = distSq;
                }

                vec3 point3 = closestPointOnLine(p2, p0, linePlaneIntersection);
                vec3 v3 = linePlaneIntersection - point3;
                distSq = dot(v3,v3);
                if (distSq < bestDist)
                {
                    referencePoint = point3;
                    bestDist = distSq;
                }

            }
        }

        center = closestPointOnLine(A, B, referencePoint);

        //////////////////////////

        float dist = dot(center - p0, N);

        float rad = sphere->radius;

        if (dist < -rad || dist > rad)
            continue;

        vec3 point0 = center - N * dist;

        vec3 c0 = cross(point0 - p0, p1 - p0);
        vec3 c1 = cross(point0 - p1, p2 - p1);
        vec3 c2 = cross(point0 - p2, p0 - p2);

        bool inside = dot(c0, N) <= 0 && dot(c1, N) <= 0 && dot(c2, N) <= 0;

        float rad2 = rad * rad;

        vec3 point1 = closestPointOnLine(p0, p1, center);
        vec3 v1 = center - point1;
        float distSq1 = dot(v1, v1);
        bool intersects = distSq1 < rad2;

        vec3 point2 = closestPointOnLine(p1, p2, center);
        vec3 v2 = center - point2;
        float distSq2 = dot(v2, v2);
        intersects |= distSq2 < rad2;

        vec3 point3 = closestPointOnLine(p2, p0, center);
        vec3 v3 = center - point3;
        float distSq3 = dot(v3, v3);
        intersects |= distSq3 < rad2;

        if (inside || intersects)
        {
            vec3 bestPoint = point0;
            vec3 intersectionVec;

            if (inside)
            {
                intersectionVec = center - point0;
            }
            else
            {
                vec3 d = center - point1;
                float bestDistSq = dot(d, d);
                bestPoint = point1;
                intersectionVec = d;

                d = center - point2;
                float distSq = dot(d,d);
                if (distSq < bestDistSq)
                {
                    bestDistSq = distSq;
                    bestPoint = point2;
                    intersectionVec = d;
                }

                d = center - point3;
                distSq = dot(d,d);
                if (distSq < bestDistSq)
                {
                    bestDistSq = distSq;
                    bestPoint = point3;
                    intersectionVec = d;
                }
            }
            float len = length(intersectionVec);
            col.collided = true;
            col.collisionPoint = vec4(bestPoint, 1.0f);
            col.collisionNormal = vec4(intersectionVec / len, 0.0f);
            col.penetrationDepth = rad - len;
            return col;
        }
    }
    collisionInfo col;
    col.collided = false;
    col.collisionNormal = vec4(0.0f);
    col.collisionPoint = vec4(0.0f);
    col.penetrationDepth = 0.0f;
    return col;

}

std::vector<collisionInfo> CollisionShape::testNearbyCollisions()
{
    std::vector<CollisionShape*> nearbyColliders = sceneManager->getNearbyColliders(this);
    std::vector<collisionInfo> cols = {};
    int numNearby = nearbyColliders.size();

    switch (collisionType)
    {
    case COLLISION_SPHERE:
        for (int i = 0; i < numNearby; i++)
        {
            CollisionShape* currentCollider = nearbyColliders[i];
            if (currentCollider == this)
                continue;
            int nearbyType = currentCollider->getType();
            collisionInfo col;
            switch (nearbyType)
            {
            case COLLISION_SPHERE:
                {
                    //std::cout << name << " Sphere col\n";
                    col = sphereSphere(currentCollider);
                    cols.push_back(col);
                    break;
                }
            case COLLISION_TRIANGLE:
                {
                    //std::cout << name << " Triangle col\n";
                    col = sphereTriangle(currentCollider);
                    cols.push_back(col);
                    break;
                }
            }
        }
        break;
            case COLLISION_CAPSULE:
               for (int i = 0; i < numNearby; i++)
        {
            CollisionShape* currentCollider = nearbyColliders[i];
            if (currentCollider == this)
                continue;
            int nearbyType = currentCollider->getType();
            collisionInfo col;
            switch (nearbyType)
            {
            case COLLISION_SPHERE:
                {
                    break;
                }
            case COLLISION_TRIANGLE:
                {
                    //std::cout << name << " Triangle col\n";
                    col = capsuleTriangle(currentCollider);
                    cols.push_back(col);
                    break;
                }
            }
        }

    }
    //std::cout << "Cols size: " << cols.size() << "\n";
    return cols;
}

void CollisionShape::drawWireframe(Camera* camera, Window* window)
{
    glMatrixMode(GL_PROJECTION);
    glm::mat4 projMatrix = camera->getProjectionMatrix(window->getAspect());
    glLoadMatrixf((const GLfloat*)&projMatrix[0]);
    glMatrixMode(GL_MODELVIEW);
    glm::mat4 MV = camera->getCameraMatrix() * getGlobalTransform();
    glLoadMatrixf((const GLfloat*)&MV[0]);
    glUseProgram(0);
    switch (collisionType)
    {
    case COLLISION_TRIANGLE:
        {
            glColor3f(1,0,0);
            glBegin(GL_LINES);
            int triangleCount = mesh->triangles.size()/3;
            for (unsigned int i = 0; i < triangleCount; i++)
            {
                unsigned int base = i * 3;
                unsigned int index0 = mesh->triangles[base];
                unsigned int index1 = mesh->triangles[base + 1];
                unsigned int index2 = mesh->triangles[base + 2];
                glm::vec4 p0 = mesh->vertices[index0];
                glm::vec4 p1 = mesh->vertices[index1];
                glm::vec4 p2 = mesh->vertices[index2];

                glVertex4fv(&p0.x);
                glVertex4fv(&p1.x);
                glVertex4fv(&p0.x);
                glVertex4fv(&p2.x);
                glVertex4fv(&p1.x);
                glVertex4fv(&p2.x);
            }
            glEnd();
            break;
        }
    case COLLISION_SPHERE:
        {
            using namespace glm;
            glColor3f(1,0,1);
            glBegin(GL_LINES);
            vec4 c = vec4(0.0f,0.0f,0.0f,1.0f);
            vec4 p0 = c + vec4(1.0f,0.0f,0.0f,0.0f) * radius * 1.1f;
            vec4 p1 = c + vec4(-1.0f,0.0f,0.0f,0.0f) * radius * 1.1f;
            vec4 p2 = c + vec4(0.0f,0.0f,1.0f,0.0f) * radius * 1.1f;
            vec4 p3 = c + vec4(0.0f,0.0f,-1.0f,0.0f) * radius * 1.1f;
            vec4 p4 = c + vec4(0.0f,1.0f,0.0f,0.0f) * radius * 1.1f;
            vec4 p5 = c + vec4(0.0f,-1.0f,0.0f,0.0f) * radius * 1.1f;

            glVertex4fv(&c.x);
            glVertex4fv(&p0.x);
            glVertex4fv(&c.x);
            glVertex4fv(&p1.x);
            glVertex4fv(&c.x);
            glVertex4fv(&p2.x);
            glVertex4fv(&c.x);
            glVertex4fv(&p3.x);
            glVertex4fv(&c.x);
            glVertex4fv(&p4.x);
            glVertex4fv(&c.x);
            glVertex4fv(&p5.x);
            glEnd();
            break;
        }
    case COLLISION_CAPSULE:
        {
            std::cout << "Capsule\n";
            using namespace glm;
            glColor3f(1,0,1);
            glBegin(GL_LINES);
            vec4 c = vec4(0.0f,0.0f,0.0f,1.0f);
            vec4 p0 = c + vec4(1.0f,0.0f,0.0f,0.0f) * radius * 1.1f;
            vec4 p1 = c + vec4(-1.0f,0.0f,0.0f,0.0f) * radius * 1.1f;
            vec4 p2 = c + vec4(0.0f,0.0f,1.0f,0.0f) * radius * 1.1f;
            vec4 p3 = c + vec4(0.0f,0.0f,-1.0f,0.0f) * radius * 1.1f;
            vec4 p4 = c + vec4(0.0f,1.0f,0.0f,0.0f) * radius * 1.1f;
            vec4 p5 = c + vec4(0.0f,-1.0f,0.0f,0.0f) * radius * 1.1f;

            glVertex4fv(&c.x);
            glVertex4fv(&p0.x);
            glVertex4fv(&c.x);
            glVertex4fv(&p1.x);
            glVertex4fv(&c.x);
            glVertex4fv(&p2.x);
            glVertex4fv(&c.x);
            glVertex4fv(&p3.x);
            glVertex4fv(&c.x);
            glVertex4fv(&p4.x);
            glVertex4fv(&c.x);
            glVertex4fv(&p5.x);

            glColor3f(0,1,1);
            c = vec4(0.0f,height,0.0f,1.0f);
            p0 = c + vec4(1.0f,0.0f,0.0f,0.0f) * radius * 1.1f;
            p1 = c + vec4(-1.0f,0.0f,0.0f,0.0f) * radius * 1.1f;
            p2 = c + vec4(0.0f,0.0f,1.0f,0.0f) * radius * 1.1f;
            p3 = c + vec4(0.0f,0.0f,-1.0f,0.0f) * radius * 1.1f;
            p4 = c + vec4(0.0f,1.0f,0.0f,0.0f) * radius * 1.1f;
            p5 = c + vec4(0.0f,-1.0f,0.0f,0.0f) * radius * 1.1f;

            glVertex4fv(&c.x);
            glVertex4fv(&p0.x);
            glVertex4fv(&c.x);
            glVertex4fv(&p1.x);
            glVertex4fv(&c.x);
            glVertex4fv(&p2.x);
            glVertex4fv(&c.x);
            glVertex4fv(&p3.x);
            glVertex4fv(&c.x);
            glVertex4fv(&p4.x);
            glVertex4fv(&c.x);
            glVertex4fv(&p5.x);
            glEnd();
            break;

        }
    }

}
