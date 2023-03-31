#ifndef TRIGGERVOLUME_H
#define TRIGGERVOLUME_H
#include <Node3D.h>
#include <CollisionShape.h>
#include <functional>

class PhysicsBody;

//Volume que manda uma mens�gem caso algum corpo fa�a contato com ele
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
        std::vector<CollisionShape*> collisionShapes;//As collision shapes que comp�e o corpo
        void recalcAABB();
        Node* receiver;//O node que deve receber a mens�gem de colis�o
        bool enabled = true;//Se o corpo est� ativo
        bool doOnce = false;//Se o corpo deve se desativar depois da primeira colis�o

};

#endif // TRIGGERVOLUME_H
