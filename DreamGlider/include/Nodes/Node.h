#ifndef NODE_H
#define NODE_H
#include <stdlib.h>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <matrices.h>
#include <string>
enum e_NodeType {NODE_TYPE_NODE = 0, NODE_TYPE_NODE_3D = 1, NODE_TYPE_MESH_3D = 2, NODE_TYPE_PHYSICS_BODY = 3, NODE_TYPE_COLLISION_SHAPE = 4};

class SceneManager;

class Node
{
    public:
        Node(std::string name);
        virtual ~Node();

        int type = NODE_TYPE_NODE;//0 = no transform, 1 = transform, 2 = 3d model

        std::string name = "Node";
        bool root = false;
        Node* parent = nullptr;
        std::vector<Node*> children;
        bool visible = true;

        virtual Node* removeChildAtIndex(std::vector<Node*>::iterator childIndex);
        Node* removeChild(Node* child);
        virtual void addChild(Node* newChild);
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

        void unsetMoved(){moved = false;}
        static SceneManager* sceneManager;
        static void setSceneManager(SceneManager* snMngr);

    protected:
        bool moved = true;
        void setMoved();

    private:
};


#endif // NODE_H
