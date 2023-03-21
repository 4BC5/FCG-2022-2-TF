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

enum e_CollisionType {COLLISION_SPHERE, COLLISION_CUBE, COLLISION_CAPSULE, COLLISION_TRIANGLE};

typedef struct
{
    bool collided = false;
    glm::vec4 collisionNormal = glm::vec4(0.0f);
    glm::vec4 collisionPoint = glm::vec4(0.0f);
    float penetrationDepth = 0.0f;
} collisionInfo;

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
        collisionInfo testAgainst(CollisionShape* col);

        void setCollisionType(int collisionType);
        void setRadius(float radius){this->radius = radius; recalcAABB();}
        void setHeight(float hight){this->height = height; recalcAABB();}
        void setWidth(float width){this->width = width; recalcAABB();}
        void setMesh(Mesh3D* mesh);

        std::vector<collisionInfo> testNearbyCollisions(PhysicsBody* bodyTest);
        void drawWireframe(Camera* camera, Window* window);

    protected:
        void recalcAABB();

    private:
        int collisionType = COLLISION_SPHERE;

        float radius = 1.0;
        float height = 1.7;
        float width = 1.0;
        Mesh3D* mesh = nullptr;

        collisionInfo testCollision(CollisionShape collider);
        std::vector<collisionInfo> sphereTriangle(CollisionShape* collider);
        collisionInfo sphereSphere(CollisionShape* collider);
        collisionInfo capsuleTriangle(CollisionShape* collider);

};

#endif // COLLISIONSHAPE_H
