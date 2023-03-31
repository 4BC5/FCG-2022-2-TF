#ifndef COLLISIONSHAPE_H
#define COLLISIONSHAPE_H
#include<glm/vec4.hpp>
#include<vector>
#include<Mesh3D.h>
#include<Nodes/Node3D.h>
#include<string>
#include<SceneManager.h>
#include<Camera.h>
#include<Window/Window.h>
#include<PhysicsBody.h>

enum e_CollisionType {COLLISION_SPHERE, COLLISION_CUBE, COLLISION_CAPSULE, COLLISION_TRIANGLE};//Tipos de colisão

typedef struct
{
    bool collided = false;
    glm::vec4 collisionNormal = glm::vec4(0.0f);
    glm::vec4 collisionPoint = glm::vec4(0.0f);
    float penetrationDepth = 0.0f;
} collisionInfo;


//Classe que define colisões físicas
class CollisionShape : public Node3D
{
    public:
        CollisionShape(std::string name);
        virtual ~CollisionShape();

        int getCollisionType(){return collisionType;}
        Mesh3D* getMesh(){return mesh;}
        float getRadius(){return radius;}
        float getHeight(){return height;}
        float getWidth(){return width;}
        int getType(){return collisionType;}
        collisionInfo testAgainst(CollisionShape* col);//Testa colisão contra outra colisão "col"

        void setCollisionType(int collisionType);//Define o tipo de colisão
        void setRadius(float radius){this->radius = radius; recalcAABB();}//Defino o raio da colisão (caso seja esféra ou capsula)
        void setHeight(float hight){this->height = height; recalcAABB();}//Define altura da colisão
        void setWidth(float width){this->width = width; recalcAABB();}//Define largura da colisão
        void setMesh(Mesh3D* mesh);//Define a Mesh3D a ser usada para colisões de tríângulo

        std::vector<collisionInfo> testNearbyCollisions(PhysicsBody* bodyTest);//Testa colisões próximas
        void drawWireframe(Camera* camera, Window* window);//Desenha o wireframe da colisão na tela para debug

    protected:
        void recalcAABB();//Recalcula a AABB caso necessário

    private:
        int collisionType = COLLISION_SPHERE;

        float radius = 1.0;
        float height = 1.7;
        float width = 1.0;
        Mesh3D* mesh = nullptr;

        collisionInfo testCollision(CollisionShape collider);//Teste de colisão geral
        std::vector<collisionInfo> sphereTriangle(CollisionShape* collider);//Teste de colisão esféra-triângulo
        collisionInfo sphereSphere(CollisionShape* collider);//Teste de colisão esféra-esféra
        collisionInfo capsuleTriangle(CollisionShape* collider);//Teste de colisão capsula-triângulo

};

#endif // COLLISIONSHAPE_H
