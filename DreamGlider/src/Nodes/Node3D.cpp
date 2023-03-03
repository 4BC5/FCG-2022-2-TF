#include <Nodes/Node3D.h>
#include <matrices.h>
#include <glm/mat4x4.hpp>
#include <sstream>

Node3D::Node3D(std::string name) : Node(name)
{
    type = 1;
    positionMatrix = mop::Matrix_Identity();
    rotationMatrix = mop::Matrix_Identity();
    scaleMatrix = mop::Matrix_Identity();
    appliedTransform = mop::Matrix_Identity();
}

Node3D::~Node3D()
{
    //dtor
}

void Node3D::applyGlobalTransform()
{
    appliedTransform = parent->getGlobalTransform() * positionMatrix * scaleMatrix * rotationMatrix;
}

void Node3D::rotateAxis(glm::vec3 axis, float phi)
{
    rotationMatrix *= mop::Matrix_Rotate(phi, glm::vec4(axis.x,axis.y,axis.z,0.0f));
}

void Node3D::rotateLocalX(float phi)
{
    rotationMatrix *= mop::Matrix_Rotate(phi, glm::vec4(transform[0][0],transform[0][1],transform[0][2],0.0f));
}

void Node3D::rotateLocalY(float phi)
{
    rotationMatrix *= mop::Matrix_Rotate(phi, glm::vec4(transform[1][0],transform[1][1],transform[2][2],0.0f));
}

void Node3D::rotateLocalZ(float phi)
{
    rotationMatrix *= mop::Matrix_Rotate(phi, glm::vec4(transform[2][0],transform[2][1],transform[2][2],0.0f));
}

void Node3D::rotateGlobalX(float phi)
{
    rotationMatrix *= mop::Matrix_Rotate_X(phi);
}

void Node3D::rotateGlobalY(float phi)
{
    //mop::PrintMatrix(getGlobalTransform());
    rotationMatrix *= mop::Matrix_Rotate_Y(phi);
}

void Node3D::rotateGlobalZ(float phi)
{
    rotationMatrix *= mop::Matrix_Rotate_Z(phi);
}

void Node3D::translate(glm::vec3 translation)
{
    positionMatrix *= mop::Matrix_Translate(translation.x, translation.y, translation.z);
}

void Node3D::globalTranslate(glm::vec3 translation)
{
    positionMatrix = positionMatrix * parent->getGlobalTransform();
    positionMatrix[3][0] += translation.x;
    positionMatrix[3][1] += translation.y;
    positionMatrix[3][2] += translation.z;
    //mop::PrintMatrix(positionMatrix);
    //positionMatrix = glm::inverse(parent->getGlobalTransform()) * appliedTransform;
}

void Node3D::scale(glm::vec3 scaleAmount)
{
    scaleMatrix *= mop::Matrix_Scale(scaleAmount.x, scaleAmount.y, scaleAmount.z);
}

glm::mat4 Node3D::getTransform()
{
    return scaleMatrix * rotationMatrix * positionMatrix;
}

glm::vec4 Node3D::getPosition()
{
    return glm::vec4(positionMatrix[3][0], positionMatrix[3][1], positionMatrix[3][2], 1.0f);
}

glm::vec4 Node3D::getBasisX()
{
    glm::mat4 basis = scaleMatrix * rotationMatrix;
    return glm::vec4(basis[0][0],basis[0][1],basis[0][2], 0.0f);
}

glm::vec4 Node3D::getBasisY()
{
    glm::mat4 basis = scaleMatrix * rotationMatrix;
    return glm::vec4(basis[1][0],basis[1][1],basis[1][2], 0.0f);
}

glm::vec4 Node3D::getBasisZ()
{
    glm::mat4 basis = scaleMatrix * rotationMatrix;
    return glm::vec4(basis[2][0],basis[2][1],basis[2][2], 0.0f);
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
