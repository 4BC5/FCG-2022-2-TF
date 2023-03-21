#include "PhysicsBody.h"
#include <algorithm>
#include <TriggerVolume.h>

PhysicsBody::PhysicsBody(std::string name) : Node3D(name)
{
    type = NODE_TYPE_PHYSICS_BODY;
    sceneManager->registerPhysicsNode(this);
}

PhysicsBody::PhysicsBody(std::string name, int type) : Node3D(name)
{
    bodyType = type;
    this->type = NODE_TYPE_PHYSICS_BODY;
    sceneManager->registerPhysicsNode(this);
}

PhysicsBody::~PhysicsBody()
{
    sceneManager->unregisterPhysicsNode(this);
}

void PhysicsBody::updateSpeed()
{
    bodySpeed = std::sqrt(bodyVelocity.x * bodyVelocity.x + bodyVelocity.y * bodyVelocity.y + bodyVelocity.z * bodyVelocity.z);
    //std::cout << "X: " << bodyVelocity.x << " Y: " << bodyVelocity.y << " Z: " << bodyVelocity.z << "\n";
}

void PhysicsBody::addChild(Node* newChild)
{
    Node::addChild(newChild);
    if (newChild->type == NODE_TYPE_COLLISION_SHAPE)
    {
        CollisionShape* newColSh = static_cast<CollisionShape*>(newChild);
        collisionShapes.push_back(newColSh);
        if (recalcAABBonAddChild)
            recalcAABB();
    }
}

void PhysicsBody::recalcAABB()
{
    aabb = AABB(1.0f,1.0f,1.0f);
    for (unsigned int i = 0; i < collisionShapes.size(); i++)
    {
        aabb.combineAABB(collisionShapes[i]->getAABB(), collisionShapes[i]->getPosition());
    }
}

Node* PhysicsBody::removeChildAtIndex(std::vector<Node*>::iterator childIndex)
{
    Node* removedC =  Node::removeChildAtIndex(childIndex);
    if (removedC->type == NODE_TYPE_COLLISION_SHAPE)
    {
        std::vector<CollisionShape*>::iterator removedCIndex = std::find(collisionShapes.begin(), collisionShapes.end(), static_cast<CollisionShape*>(removedC));
        collisionShapes.erase(removedCIndex);
    }
    return removedC;
}

void PhysicsBody::doMovement(float deltaTime)
{
    using namespace glm;
    updateSpeed();
    vec4 globalPosition = getGlobalPosition();

    for (unsigned int i = 0; i < children.size(); i++)
    {
        if (children[i]->type == NODE_TYPE_COLLISION_SHAPE)
        {
            CollisionShape* currentShape = static_cast<CollisionShape*>(children[i]);
            std::vector<collisionInfo> cols = currentShape->testNearbyCollisions(this);

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

void PhysicsBody::testTriggers()
{
    std::vector<TriggerVolume*> triggers = sceneManager->getNearbyTriggers(aabb);
    unsigned int triggerCount = triggers.size();
    for (unsigned int i= 0; i < triggerCount; i++)
    {
        std::vector<CollisionShape*> colShapes = triggers[i]->getCollisionShapes();
        unsigned int shapeCount = colShapes.size();
        bool collided = false;
        for(unsigned int j = 0; j < shapeCount; j++)
        {
            CollisionShape* currentCol = colShapes[j];
            for (unsigned int k = 0; k < collisionShapes.size(); k++)
            {
                collisionInfo cInf = collisionShapes[j]->testAgainst(currentCol);
                if (cInf.collided)
                {
                    triggers[i]->onCollision(this);
                    collided = true;
                    break;
                }
            }
            if (collided)
                break;
        }
        if (collided)
        {
            triggers[i]->onCollision(this);
        }
    }
}

void PhysicsBody::addAcceleration(glm::vec4 accel)
{
    acceleration = accel;
}
