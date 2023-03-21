#ifndef TRIGGERVOLUME_H
#define TRIGGERVOLUME_H
#include <Node3D.h>
#include <CollisionShape.h>
#include <functional>

class PhysicsBody;

class TriggerVolume : public Node3D
{
    public:
        TriggerVolume(const std::string& name);
        virtual ~TriggerVolume();

        void addChild(Node* newChild);
        void onCollision(PhysicsBody* collidingNode);

        void setOnCollisionReceiver(Node* receiver){this->receiver = receiver;}

        const std::vector<CollisionShape*>& getCollisionShapes(){return collisionShapes;}

    protected:

    private:
        std::vector<CollisionShape*> collisionShapes;
        void recalcAABB();
        Node* receiver;

};

#endif // TRIGGERVOLUME_H
