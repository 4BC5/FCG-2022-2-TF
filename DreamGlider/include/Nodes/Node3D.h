#ifndef NODE3D_H
#define NODE3D_H

#include <Nodes/Node.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <matrices.h>


class Node3D : public Node
{
    public:
        Node3D(std::string name);
        virtual ~Node3D();

        glm::mat4 transform = mop::Matrix_Identity();
        glm::mat4 positionMatrix = mop::Matrix_Identity();
        glm::mat4 rotationMatrix = mop::Matrix_Identity();
        glm::mat4 scaleMatrix = mop::Matrix_Identity();
        glm::mat4 appliedTransform = mop::Matrix_Identity();
        glm::mat4 getTransform();
        glm::mat4 getGlobalTransform();

        void rotateAxis(glm::vec3 axis, float phi);

        void rotateLocalX(float phi);
        void rotateLocalY(float phi);
        void rotateLocalZ(float phi);

        void rotateGlobalX(float phi);
        void rotateGlobalY(float phi);
        void rotateGlobalZ(float phi);

        void translate(glm::vec3 translation);
        void globalTranslate(glm::vec3 translation);

        void scale(glm::vec3 scaleAmount);

        void setPosition(glm::vec3 position);
        void setGlobalPosition(glm::vec3 position);

        glm::vec4 getPosition();
        glm::vec4 getGlobalPosition();
        glm::vec4 getBasisX();
        glm::vec4 getBasisY();
        glm::vec4 getBasisZ();
        glm::vec4 getGlobalBasisX();
        glm::vec4 getGlobalBasisY();
        glm::vec4 getGlobalBasisZ();


        void setAppliedTransform(glm::mat4 newTransform);
        void applyGlobalTransform();
    protected:

    private:
};

#endif // NODE3D_H
