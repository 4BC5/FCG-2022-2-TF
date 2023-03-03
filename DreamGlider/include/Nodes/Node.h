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

        Node* removeChild(std::vector<Node*>::iterator childIndex);
        void addChild(Node* newChild);
        void queueFree();
        std::vector<Node*>::iterator getChildIndex(Node* child);

        Node* getParent();
        std::vector<Node*> getChildren();

        virtual void sceneSetup();

        //Node 3D methods
        virtual glm::mat4 getTransform(){return mop::Matrix_Identity();}
        virtual void setAppliedTransform(glm::mat4 newTransform){}
        virtual glm::mat4 getGlobalTransform()
        {
            if (!root)
                return parent->getGlobalTransform();
            else
                return mop::Matrix_Identity();
        }

        virtual void rotateAxis(glm::vec3 axis, float phi){};
        virtual void rotateLocalX(float phi){};
        virtual void rotateLocalY(float phi){};
        virtual void rotateLocalZ(float phi){};
        virtual void rotateGlobalX(float phi){};
        virtual void rotateGlobalY(float phi){};
        virtual void rotateGlobalZ(float phi){};
        virtual void translate(glm::vec3 translation){};
        virtual void scale(glm::vec3 scaleAmount){};

        //NodeMesh3D methods
        virtual std::vector<glm::vec4>* getMeshPoints(){return nullptr;}
        virtual std::vector<glm::vec4>* getMeshNormals(){return nullptr;}
        virtual std::vector<GLuint>* getMeshTriangles(){return nullptr;}
        virtual std::vector<glm::vec2>* getMeshUVs(){return nullptr;}
        virtual void loadMesh(std::string meshLocation){}
        virtual std::string getShaderPath(){return "";}
        virtual GLuint getVertexCount(){return 0;}
        virtual GLuint getVAO(){return 0;}
        virtual void setVAO(GLuint newVAO){};

    protected:

    private:
};

#endif // NODE_H
