#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <Nodes/Node.h>
#include <Nodes/Node3D.h>

class SceneManager
{
    public:
        SceneManager(Node* root);
        virtual ~SceneManager();

        Node* sceneRoot = nullptr;

        void applyTransforms();

    protected:

    private:
};

#endif // SCENEMANAGER_H
