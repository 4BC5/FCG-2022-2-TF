#ifndef NODE_H
#define NODE_H
#include <stdlib.h>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <matrices.h>
#include <string>

class Node
{
    public:
        Node(std::string name);
        virtual ~Node();

        int type = 0;//0 = no transform, 1 = transform, 2 = 3d model

        std::string name = "Node";
        bool root = false;
        Node* parent = nullptr;
        std::vector<Node*> children;
        bool visible = true;

        Node* removeChildAtIndex(std::vector<Node*>::iterator childIndex);
        Node* removeChild(Node* child);
        void addChild(Node* newChild);
        void queueFree();
        std::vector<Node*>::iterator getChildIndex(Node* child);

        Node* getParent();
        std::vector<Node*> getChildren();

        virtual void sceneSetup();

        //Node 3D methods
        virtual glm::mat4 getGlobalTransform()
        {
            if (!root)
                return parent->getGlobalTransform();
            else
                return mop::Matrix_Identity();
        }

    protected:

    private:
};

#endif // NODE_H
