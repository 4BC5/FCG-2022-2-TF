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

void Node3D::applyGlobalTransform()
{
    appliedTransform = transform * parent->getGlobalTransform();
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

void Node3D::globalTranslate(glm::vec3 translation)
{
    applyGlobalTransform();
    appliedTransform[3][0] += translation.x;
    appliedTransform[3][1] += translation.y;
    appliedTransform[3][2] += translation.z;
    transform = glm::inverse(parent->getGlobalTransform()) * appliedTransform;
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

glm::vec4 Node3D::getGlobalBasisX()
{
    return glm::vec4(appliedTransform[0][0],appliedTransform[0][1],appliedTransform[0][2], 0.0f);
}

glm::vec4 Node3D::getGlobalBasisY()
{
    return glm::vec4(appliedTransform[1][0],appliedTransform[1][1],appliedTransform[1][2], 0.0f);
}

glm::vec4 Node3D::getGlobalBasisZ()
{
    return glm::vec4(appliedTransform[2][0],appliedTransform[2][1],appliedTransform[2][2], 0.0f);
}

glm::mat4 Node3D::getGlobalTransform()
{
    return appliedTransform;
}

glm::vec4 Node3D::getGlobalPosition()
{
    return glm::vec4(appliedTransform[3][0], appliedTransform[3][1], appliedTransform[3][2], appliedTransform[3][3]);
}

void Node3D::setAppliedTransform(glm::mat4 newTransform)
{
    appliedTransform = newTransform;
}
