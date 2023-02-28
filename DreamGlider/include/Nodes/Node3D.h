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
        Node3D();
        virtual ~Node3D();

        glm::mat4 transform = mop::Matrix_Identity();
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

        void scale(glm::vec3 scaleAmount);

        glm::vec4 getPosition();
        glm::vec4 getGlobalPosition();
        glm::vec4 getBasisX();
        glm::vec4 getBasisY();
        glm::vec4 getBasisZ();

        void setAppliedTransform(glm::mat4 newTransform);
    protected:

    private:
};

#endif // NODE3D_H
