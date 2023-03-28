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
        void setDoOnce(bool doOnce){this->doOnce = doOnce;}
        void setEnabled(bool enabled){this->enabled = enabled;}

        const std::vector<CollisionShape*>& getCollisionShapes(){return collisionShapes;}

    protected:

    private:
        std::vector<CollisionShape*> collisionShapes;
        void recalcAABB();
        Node* receiver;
        bool enabled = true;
        bool doOnce = false;

};

#endif // TRIGGERVOLUME_H
