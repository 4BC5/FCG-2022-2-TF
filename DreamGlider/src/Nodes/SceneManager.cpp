#include "SceneManager.h"

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

void SceneManager::applyTransforms()
{
    applyTransformsRecursive(sceneRoot);
}
