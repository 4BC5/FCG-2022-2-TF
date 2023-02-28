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
    if (object->type > 0 && object->root == false)
    {
        object->setAppliedTransform(object->getTransform() * object->getParent()->getGlobalTransform());
        for (int i = 0; i < object->children.size(); i++)
        {
            applyTransformsRecursive(object->children[i]);
        }
    }
}

void SceneManager::applyTransforms()
{
    applyTransformsRecursive(sceneRoot);
}
