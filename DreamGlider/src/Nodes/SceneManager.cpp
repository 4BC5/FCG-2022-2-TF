#include "SceneManager.h"
#include <PhysicsBody.h>
#include <algorithm>

float SceneManager::deltaTime = 0.0f;

SceneManager::SceneManager(Node* root)
{
    sceneRoot = root;
}

SceneManager::~SceneManager()
{
    //dtor
}

void applyTransformsRecursive(Node* object)
{
    if (object->type > 0)
    {
        Node3D* node = static_cast<Node3D*>(object);
        if (node->root == false)
            node->applyGlobalTransform();
    }
    object->unsetMoved();
    for (unsigned int i = 0; i < object->children.size(); i++)
    {
            applyTransformsRecursive(object->children[i]);
    }
}

void SceneManager::applyPhysics(float deltaTime)
{
    SceneManager::setDeltaTime(deltaTime);
    int dynamicBodiesCount = dynamicBodies.size();
    for (int i = 0; i < dynamicBodiesCount; i++)
    {
        int index = dynamicBodies[i];
        physBodies[index]->doMovement(deltaTime);
    }
}

void SceneManager::applyTransforms()
{
    applyTransformsRecursive(sceneRoot);
}

void SceneManager::registerPhysicsNode(PhysicsBody* node)
{
    std::cout << "Physics body: \"" << node->name << "\" registered\n";
    physBodies.push_back(node);
    if (node->getBodyType() == PHYS_BODY_KINEMATIC)
    {
        std::cout << "Registered as dynamic\n";
        registerDynamicBody(physBodies.size() - 1);
    }
}

void SceneManager::registerDynamicBody(int id)
{
    dynamicBodies.push_back(id);
}

void SceneManager::unregisterPhysicsNode(PhysicsBody* node)
{
    std::vector<PhysicsBody*>::iterator bodyPos = std::find(physBodies.begin(), physBodies.end(), node);
    physBodies.erase(bodyPos);
    if (node->getBodyType() == PHYS_BODY_KINEMATIC)
    {
        std::vector<int>::iterator dynamicPos = std::find(dynamicBodies.begin(), dynamicBodies.end(), std::distance(physBodies.begin(), bodyPos));
        dynamicBodies.erase(dynamicPos);
    }
}

std::vector<CollisionShape*> SceneManager::getNearbyColliders(PhysicsBody* body)
{
    std::vector<CollisionShape*> nearbyColliders;
    AABB thisAABB = body->getAABB();
    unsigned int bodyCount = physBodies.size();
    for (unsigned int i = 0; i < bodyCount; i++)
    {
        if (physBodies[i] != body && thisAABB.AABBtoAABBtest(physBodies[i]->getAABB()))
        {
            //std::cout << physBodies[i]->name << "\n";
            std::vector<CollisionShape*> found = physBodies[i]->getCollisionShapes();
            nearbyColliders.insert(nearbyColliders.end(), found.begin(), found.end());
        }
    }
    return nearbyColliders;
}
