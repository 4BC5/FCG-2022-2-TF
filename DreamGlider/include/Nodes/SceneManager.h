#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <Nodes/Node3D.h>
//#include <CollisionShape.h>
#include <vector>
//#include <PhysicsBody.h>

class CollisionShape;
class PhysicsBody;

class SceneManager
{
    public:
        SceneManager(Node* root);
        virtual ~SceneManager();

        Node* sceneRoot = nullptr;

        void applyTransforms();
        void applyPhysics(float deltaTime);
        std::vector<CollisionShape*>& getNearbyColliders(CollisionShape*);
        void registerPhysicsNode(PhysicsBody* node);
        void registerCollisionNode(CollisionShape* node);
        static float getDeltaTime(){return deltaTime;}

    protected:
        static void setDeltaTime(float dt){deltaTime = dt;}
        static float deltaTime;

    private:
        std::vector<PhysicsBody*> physBodies;
        std::vector<CollisionShape*> collisionShapes;
        std::vector<int> dynamicBodies;

        void registerDynamicBody(int id);
};

#endif // SCENEMANAGER_H
