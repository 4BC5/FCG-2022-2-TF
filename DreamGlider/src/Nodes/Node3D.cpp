#include <Nodes/Node3D.h>
#include <matrices.h>
#include <glm/mat4x4.hpp>
#include <sstream>

Node3D::Node3D()
{
    type = 1;
    transform = mop::Matrix_Identity();
    appliedTransform = mop::Matrix_Identity();
}

Node3D::~Node3D()
{
    //dtor
}


void Node3D::rotateAxis(glm::vec3 axis, float phi)
{
    transform *= mop::Matrix_Rotate(phi, glm::vec4(axis.x,axis.y,axis.z,0.0f));
}

void Node3D::rotateLocalX(float phi)
{
    transform *= mop::Matrix_Rotate(phi, glm::vec4(transform[0][0],transform[0][1],transform[0][2],0.0f));
}

void Node3D::rotateLocalY(float phi)
{
    transform *= mop::Matrix_Rotate(phi, glm::vec4(transform[1][0],transform[1][1],transform[2][2],0.0f));
}

void Node3D::rotateLocalZ(float phi)
{
    transform *= mop::Matrix_Rotate(phi, glm::vec4(transform[2][0],transform[2][1],transform[2][2],0.0f));
}

void Node3D::rotateGlobalX(float phi)
{
    transform *= mop::Matrix_Rotate_X(phi);
}

void Node3D::rotateGlobalY(float phi)
{
    transform *= mop::Matrix_Rotate_Y(phi);
}

void Node3D::rotateGlobalZ(float phi)
{
    transform *= mop::Matrix_Rotate_Z(phi);
}

void Node3D::translate(glm::vec3 translation)
{
    transform *= mop::Matrix_Translate(translation.x, translation.y, translation.z);
}

void Node3D::scale(glm::vec3 scaleAmount)
{
    transform *= mop::Matrix_Scale(scaleAmount.x, scaleAmount.y, scaleAmount.z);
}

glm::mat4 Node3D::getTransform()
{
    return transform;
}

glm::vec4 Node3D::getPosition()
{
    return glm::vec4(transform[3][0], transform[3][1], transform[3][2], 1.0f);
}

glm::vec4 Node3D::getBasisX()
{
    return glm::vec4(transform[0][0],transform[0][1],transform[0][2], 0.0f);
}

glm::vec4 Node3D::getBasisY()
{
    return glm::vec4(transform[1][0],transform[1][1],transform[1][2], 0.0f);
}

glm::vec4 Node3D::getBasisZ()
{
    return glm::vec4(transform[2][0],transform[2][1],transform[2][2], 0.0f);
}

glm::mat4 Node3D::getGlobalTransform()
{
    return appliedTransform;
}
