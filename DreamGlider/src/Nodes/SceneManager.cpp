#include "SceneManager.h"
#include <PhysicsBody.h>

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
        for (unsigned int i = 0; i < node->children.size(); i++)
        {
            applyTransformsRecursive(node->children[i]);
        }
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
void SceneManager::registerCollisionNode(CollisionShape* node)
{
    collisionShapes.push_back(node);
}

void SceneManager::registerDynamicBody(int id)
{
    dynamicBodies.push_back(id);
}

std::vector<CollisionShape*>& SceneManager::getNearbyColliders(CollisionShape*)
{
    return collisionShapes;
}
