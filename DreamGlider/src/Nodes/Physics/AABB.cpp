#include "AABB.h"
#include <Renderer/Camera/Camera.h>
#include <Window/Window.h>

AABB::AABB(Mesh3D* mesh)
{
    calculateFromMesh(mesh);
}

AABB::AABB(float height, float width, float depth)
{
    using namespace glm;
    maxPoint = vec3(width, height, depth);
    minPoint = vec3(-width, -height, -depth);
}

AABB::AABB(glm::vec3 center, float height, float width, float depth)
{
    using namespace glm;
    maxPoint = vec3(width, height, depth) + center;
    minPoint = vec3(-width, -height, -depth) + center;
}

AABB::~AABB()
{
    //dtor
}

std::vector<glm::vec3> AABB::getCorners(AABB& aabb)
{
    using namespace glm;

    vec3 maxP = aabb.maxPoint;
    vec3 minP = aabb.minPoint;
    std::vector<vec3> points;
    points.resize(8);

    points[0] = maxP;
    points[1] = vec3(maxP.x, maxP.y, minP.z);
    points[2] = vec3(maxP.x, minP.y, minP.z);
    points[3] = vec3(maxP.x, minP.y, maxP.z);
    points[4] = vec3(minP.x, minP.y, maxP.z);
    points[5] = minP;
    points[6] = vec3(minP.x, maxP.y, minP.z);
    points[7] = vec3(minP.x, maxP.y, maxP.z);

    return points;
}

void AABB::calculateFromMesh(Mesh3D* mesh)
{
    using namespace glm;
    maxPoint = vec3(-1000000.0f);
    minPoint = vec3(1000000.0f);

    int vertexCount = mesh->vertices.size();
    for (int i = 0; i < vertexCount; i++)
    {
        vec4 cV = mesh->vertices[i];
        maxPoint.x = cV.x > maxPoint.x ? cV.x : maxPoint.x;
        maxPoint.y = cV.y > maxPoint.y ? cV.y : maxPoint.y;
        maxPoint.z = cV.z > maxPoint.z ? cV.z : maxPoint.z;
        minPoint.x = cV.x < minPoint.x ? cV.x : minPoint.x;
        minPoint.y = cV.y < minPoint.y ? cV.y : minPoint.y;
        minPoint.z = cV.z < minPoint.z ? cV.z : minPoint.z;
    }
}

void AABB::expandAABB(glm::vec3 expansion)
{
    maxPoint += expansion;
    minPoint -= expansion;
}

//glm::vec3 getMaxPoint();
//glm::vec3 getMinPoint();

bool AABB::AABBtoAABBtest(AABB& intersectWith)
{
    using namespace glm;
    vec3 oMax = intersectWith.getMaxPoint();
    vec3 oMin = intersectWith.getMinPoint();
    vec3 tMax = getMaxPoint();
    vec3 tMin = getMinPoint();

    return (tMax.x > oMin.x && tMin.x < oMax.x &&
            tMax.y > oMin.y && tMin.y < oMax.y &&
            tMax.z > oMin.z && tMin.z < oMax.z);
}

void AABB::updateTransform(glm::mat4 trs)
{
    using namespace glm;

    vec4 p0 = trs * vec4(maxPoint, 1.0f);
    vec4 p1 = trs * vec4(maxPoint.x, maxPoint.y, minPoint.z, 1.0f);
    vec4 p2 = trs * vec4(maxPoint.x, minPoint.y, minPoint.z, 1.0f);
    vec4 p3 = trs * vec4(maxPoint.x, minPoint.y, maxPoint.z, 1.0f);
    vec4 p4 = trs * vec4(minPoint.x, minPoint.y, maxPoint.z, 1.0f);
    vec4 p5 = trs * vec4(minPoint,1.0f);
    vec4 p6 = trs * vec4(minPoint.x, maxPoint.y, minPoint.z, 1.0f);
    vec4 p7 = trs * vec4(minPoint.x, maxPoint.y, maxPoint.z, 1.0f);

    vec3 points[] = {vec3(p0), vec3(p1), vec3(p2), vec3(p3), vec3(p4), vec3(p5), vec3(p6), vec3(p7)};

    transformedMaxPoint = vec3(p0);
    transformedMinPoint = vec3(p5);

    for (unsigned int i = 0; i < 9; i++)
    {
        vec3 cV = points[i];
        transformedMaxPoint.x = cV.x > transformedMaxPoint.x ? cV.x : transformedMaxPoint.x;
        transformedMaxPoint.y = cV.y > transformedMaxPoint.y ? cV.y : transformedMaxPoint.y;
        transformedMaxPoint.z = cV.z > transformedMaxPoint.z ? cV.z : transformedMaxPoint.z;
        transformedMinPoint.x = cV.x < transformedMinPoint.x ? cV.x : transformedMinPoint.x;
        transformedMinPoint.y = cV.y < transformedMinPoint.y ? cV.y : transformedMinPoint.y;
        transformedMinPoint.z = cV.z < transformedMinPoint.z ? cV.z : transformedMinPoint.z;
    }
}

void AABB::drawAABB(Camera* camera, Window* window)
{
    glMatrixMode(GL_PROJECTION);
    glm::mat4 projMatrix = camera->getProjectionMatrix(window->getAspect());
    glLoadMatrixf((const GLfloat*)&projMatrix[0]);
    glMatrixMode(GL_MODELVIEW);
    glm::mat4 MV = camera->getCameraMatrix();
    glLoadMatrixf((const GLfloat*)&MV[0]);
    glUseProgram(0);
    {
        using namespace glm;
        glBegin(GL_LINES);
        vec4 p0 = vec4(transformedMaxPoint, 1.0f);
        vec4 p1 = vec4(transformedMaxPoint.x, transformedMaxPoint.y, transformedMinPoint.z, 1.0f);
        vec4 p2 = vec4(transformedMaxPoint.x, transformedMinPoint.y, transformedMinPoint.z, 1.0f);
        vec4 p3 = vec4(transformedMaxPoint.x, transformedMinPoint.y, transformedMaxPoint.z, 1.0f);
        vec4 p4 = vec4(transformedMinPoint.x, transformedMinPoint.y, transformedMaxPoint.z, 1.0f);
        vec4 p5 = vec4(transformedMinPoint, 1.0f);
        vec4 p6 = vec4(transformedMinPoint.x, transformedMaxPoint.y, transformedMinPoint.z, 1.0f);
        vec4 p7 = vec4(transformedMinPoint.x, transformedMaxPoint.y, transformedMaxPoint.z, 1.0f);

        glVertex4fv(&p0.x);
        glVertex4fv(&p1.x);

        glVertex4fv(&p1.x);
        glVertex4fv(&p2.x);

        glVertex4fv(&p2.x);
        glVertex4fv(&p3.x);

        glVertex4fv(&p3.x);
        glVertex4fv(&p4.x);

        glVertex4fv(&p4.x);
        glVertex4fv(&p5.x);

        glVertex4fv(&p5.x);
        glVertex4fv(&p6.x);

        glVertex4fv(&p6.x);
        glVertex4fv(&p7.x);

        glVertex4fv(&p7.x);
        glVertex4fv(&p4.x);

        glVertex4fv(&p7.x);
        glVertex4fv(&p0.x);

        glVertex4fv(&p6.x);
        glVertex4fv(&p1.x);

        glVertex4fv(&p5.x);
        glVertex4fv(&p2.x);

        glVertex4fv(&p3.x);
        glVertex4fv(&p0.x);

        glEnd();
    }
}

void AABB::combineAABB(AABB comb, glm::vec3 center)
{
    using namespace glm;
    std::vector<vec3> points = getCorners(comb);

    for (unsigned int i = 0; i < points.size(); i++)
    {
        vec3 cV = points[i] + center;
        maxPoint.x = cV.x > maxPoint.x ? cV.x : maxPoint.x;
        maxPoint.y = cV.y > maxPoint.y ? cV.y : maxPoint.y;
        maxPoint.z = cV.z > maxPoint.z ? cV.z : maxPoint.z;
        minPoint.x = cV.x < minPoint.x ? cV.x : minPoint.x;
        minPoint.y = cV.y < minPoint.y ? cV.y : minPoint.y;
        minPoint.z = cV.z < minPoint.z ? cV.z : minPoint.z;
    }
}
