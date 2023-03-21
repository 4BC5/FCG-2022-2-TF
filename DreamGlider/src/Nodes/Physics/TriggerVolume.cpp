#include "TriggerVolume.h"
#include <PhysicsBody.h>

TriggerVolume::TriggerVolume(const std::string& name) : Node3D(name)
{
    type = NODE_TYPE_TRIGGER_VOLUME;
    aabb = AABB(1.0f,1.0f,1.0f);
    sceneManager->registerTrigger(this);
}

TriggerVolume::~TriggerVolume()
{
    sceneManager->unregisterTrigger(this);
}

void TriggerVolume::addChild(Node* newChild)
{
    Node::addChild(newChild);
    if (newChild->type == NODE_TYPE_COLLISION_SHAPE)
    {
        CollisionShape* newColSh = static_cast<CollisionShape*>(newChild);
        collisionShapes.push_back(newColSh);
        recalcAABB();
    }
}

void TriggerVolume::recalcAABB()
{
    aabb = AABB(1.0f,1.0f,1.0f);
    for (unsigned int i = 0; i < collisionShapes.size(); i++)
    {
        aabb.combineAABB(collisionShapes[i]->getAABB(), collisionShapes[i]->getPosition());
    }
}

void TriggerVolume::onCollision(PhysicsBody* collidingNode)
{
    receiver->onReceiveMessage("trigger", "PhysicsBody", static_cast<void*>(collidingNode));
}
