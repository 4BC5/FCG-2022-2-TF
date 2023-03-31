#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H
#include <Node3D.h>
#include <CollisionShape.h>

enum e_bodyType {PHYS_BODY_STATIC, PHYS_BODY_KINEMATIC};
//Classe que define corpos f�sicos que sofrer�o colis�es
//Podem ser est�ticos (n�o recebem updates) ou kinematic (recebem updates todo frame de f�sica)
class PhysicsBody : public Node3D
{
    public:
        PhysicsBody(std::string name);
        PhysicsBody(std::string name, int type);
        virtual ~PhysicsBody();

        void addChild(Node* newChild);//Override de add child e removeChild para detectar collision shapes
        Node* removeChildAtIndex(std::vector<Node*>::iterator childIndex);

        void setBodyType(int bodyType){this->bodyType = bodyType;}
        void setGravity();//N�o utilizado no momento

        virtual void doMovement(float deltaTime);//Tick de f�sica
        virtual void addAcceleration(glm::vec4 accel);//Adicionar acelera��o

        int getBodyType(){return bodyType;}

        const std::vector<CollisionShape*>& getCollisionShapes(){return collisionShapes;}//Retorna as CollisionShapes que fazer parte do corpo

    protected:
        std::vector<CollisionShape*> collisionShapes;//As collision shapes que fazem parte do corpo

        void updateSpeed();//Update do contador de velocidade
        void recalcAABB();//Recalcula a AABB quando collison meshes s�o adicionados
        void testTriggers();//Testa contra triggers

        int maxCollisionsPerFrame = 4;//M�ximo de colis�es por frame que podem acontecer
        float bodySpeed = 0.0;//Contador de velocidade do corpo
        int bodyType = PHYS_BODY_STATIC;//Tipo de corpo (static ou kinematic)
        glm::vec4 gravity = glm::vec4(0.0f, -9.8f, 0.0f, 0.0f);//Gravidade applicada no corpo
        glm::vec4 bodyVelocity = glm::vec4(0.0);//Vetor velocidade do corpo
        glm::vec4 acceleration = glm::vec4(0.0f);//Acelera��o a ser aplicada ao corpo
        bool recalcAABBonAddChild = true;//Recalcular AABB nos filhos a cada tick

    private:
};

#endif // PHYSICSBODY_H
