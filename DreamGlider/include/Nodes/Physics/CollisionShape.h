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

enum e_CollisionType {COLLISION_SPHERE, COLLISION_CUBE, COLLISION_CAPSULE, COLLISION_TRIANGLE};//Tipos de colis�o

typedef struct
{
    bool collided = false;
    glm::vec4 collisionNormal = glm::vec4(0.0f);
    glm::vec4 collisionPoint = glm::vec4(0.0f);
    float penetrationDepth = 0.0f;
} collisionInfo;


//Classe que define colis�es f�sicas
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
        collisionInfo testAgainst(CollisionShape* col);//Testa colis�o contra outra colis�o "col"

        void setCollisionType(int collisionType);//Define o tipo de colis�o
        void setRadius(float radius){this->radius = radius; recalcAABB();}//Defino o raio da colis�o (caso seja esf�ra ou capsula)
        void setHeight(float hight){this->height = height; recalcAABB();}//Define altura da colis�o
        void setWidth(float width){this->width = width; recalcAABB();}//Define largura da colis�o
        void setMesh(Mesh3D* mesh);//Define a Mesh3D a ser usada para colis�es de tr��ngulo

        std::vector<collisionInfo> testNearbyCollisions(PhysicsBody* bodyTest);//Testa colis�es pr�ximas
        void drawWireframe(Camera* camera, Window* window);//Desenha o wireframe da colis�o na tela para debug

    protected:
        void recalcAABB();//Recalcula a AABB caso necess�rio

    private:
        int collisionType = COLLISION_SPHERE;

        float radius = 1.0;
        float height = 1.7;
        float width = 1.0;
        Mesh3D* mesh = nullptr;

        collisionInfo testCollision(CollisionShape collider);//Teste de colis�o geral
        std::vector<collisionInfo> sphereTriangle(CollisionShape* collider);//Teste de colis�o esf�ra-tri�ngulo
        collisionInfo sphereSphere(CollisionShape* collider);//Teste de colis�o esf�ra-esf�ra
        collisionInfo capsuleTriangle(CollisionShape* collider);//Teste de colis�o capsula-tri�ngulo

};

#endif // COLLISIONSHAPE_H
