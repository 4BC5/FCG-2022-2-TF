#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(std::string name) : Node3D(name)
{
    type = NODE_TYPE_PHYSICS_BODY;
    sceneManager->registerPhysicsNode(this);
}

PhysicsBody::PhysicsBody(std::string name, int type) : Node3D(name)
{
    bodyType = type;
    type = NODE_TYPE_PHYSICS_BODY;
    sceneManager->registerPhysicsNode(this);
}

PhysicsBody::~PhysicsBody()
{
    //dtor
}

void PhysicsBody::updateSpeed()
{
    bodySpeed = std::sqrt(bodyVelocity.x * bodyVelocity.x + bodyVelocity.y * bodyVelocity.y + bodyVelocity.z * bodyVelocity.z);
    //std::cout << "X: " << bodyVelocity.x << " Y: " << bodyVelocity.y << " Z: " << bodyVelocity.z << "\n";
}


void PhysicsBody::doMovement(float deltaTime)
{
    using namespace glm;
    updateSpeed();
    vec4 globalPosition = getGlobalPosition();

    for (int i = 0; i < children.size(); i++)
    {
        if (children[i]->type == NODE_TYPE_COLLISION_SHAPE)
        {
            CollisionShape* currentShape = static_cast<CollisionShape*>(children[i]);
            std::vector<collisionInfo> cols = currentShape->testNearbyCollisions();

            //std::cout << "Collided:" << cols[i].collided << "\n";

            int colRange = cols.size();
            for (int i = 0; i < colRange; i++)
            {
                collisionInfo col = cols[i];
                if (col.collided)
                {
                    globalPosition += col.collisionNormal * col.penetrationDepth;
                    bodyVelocity = reflect(bodyVelocity, col.collisionNormal) * 0.5f;
                }
            }
        }
    }
    bodyVelocity = bodyVelocity + gravity * deltaTime;
    bodyVelocity += acceleration * deltaTime;
    globalPosition += bodyVelocity * deltaTime;
    setGlobalPosition(vec3(globalPosition));
}

void PhysicsBody::addAcceleration(glm::vec4 accel)
{
    acceleration = accel;
}
