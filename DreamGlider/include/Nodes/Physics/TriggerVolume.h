#ifndef TRIGGERVOLUME_H
#define TRIGGERVOLUME_H
#include <Node3D.h>
#include <CollisionShape.h>
#include <functional>

class PhysicsBody;

//Volume que manda uma menságem caso algum corpo faça contato com ele
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
        std::vector<CollisionShape*> collisionShapes;//As collision shapes que compõe o corpo
        void recalcAABB();
        Node* receiver;//O node que deve receber a menságem de colisão
        bool enabled = true;//Se o corpo está ativo
        bool doOnce = false;//Se o corpo deve se desativar depois da primeira colisão

};

#endif // TRIGGERVOLUME_H
