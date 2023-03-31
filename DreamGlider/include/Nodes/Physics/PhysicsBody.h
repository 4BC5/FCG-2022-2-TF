#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H
#include <Node3D.h>
#include <CollisionShape.h>

enum e_bodyType {PHYS_BODY_STATIC, PHYS_BODY_KINEMATIC};
//Classe que define corpos físicos que sofrerão colisões
//Podem ser estáticos (não recebem updates) ou kinematic (recebem updates todo frame de física)
class PhysicsBody : public Node3D
{
    public:
        PhysicsBody(std::string name);
        PhysicsBody(std::string name, int type);
        virtual ~PhysicsBody();

        void addChild(Node* newChild);//Override de add child e removeChild para detectar collision shapes
        Node* removeChildAtIndex(std::vector<Node*>::iterator childIndex);

        void setBodyType(int bodyType){this->bodyType = bodyType;}
        void setGravity();//Não utilizado no momento

        virtual void doMovement(float deltaTime);//Tick de física
        virtual void addAcceleration(glm::vec4 accel);//Adicionar aceleração

        int getBodyType(){return bodyType;}

        const std::vector<CollisionShape*>& getCollisionShapes(){return collisionShapes;}//Retorna as CollisionShapes que fazer parte do corpo

    protected:
        std::vector<CollisionShape*> collisionShapes;//As collision shapes que fazem parte do corpo

        void updateSpeed();//Update do contador de velocidade
        void recalcAABB();//Recalcula a AABB quando collison meshes são adicionados
        void testTriggers();//Testa contra triggers

        int maxCollisionsPerFrame = 4;//Máximo de colisões por frame que podem acontecer
        float bodySpeed = 0.0;//Contador de velocidade do corpo
        int bodyType = PHYS_BODY_STATIC;//Tipo de corpo (static ou kinematic)
        glm::vec4 gravity = glm::vec4(0.0f, -9.8f, 0.0f, 0.0f);//Gravidade applicada no corpo
        glm::vec4 bodyVelocity = glm::vec4(0.0);//Vetor velocidade do corpo
        glm::vec4 acceleration = glm::vec4(0.0f);//Aceleração a ser aplicada ao corpo
        bool recalcAABBonAddChild = true;//Recalcular AABB nos filhos a cada tick

    private:
};

#endif // PHYSICSBODY_H
