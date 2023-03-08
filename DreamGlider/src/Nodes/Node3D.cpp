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
    rotationMatrix *= mop::Matrix_Rotate(phi,getBasisX());
}

void Node3D::rotateLocalY(float phi)
{
    rotationMatrix *= mop::Matrix_Rotate(phi,getBasisY());
}

void Node3D::rotateLocalZ(float phi)
{
    rotationMatrix *= mop::Matrix_Rotate(phi,getBasisZ());
}

void Node3D::rotateGlobalX(float phi)
{
    glm::vec4 axis = glm::vec4(1.0f,0.0f,0.0f,0.0f) * appliedTransform;
    rotationMatrix *= mop::Matrix_Rotate(phi, axis);
}

void Node3D::rotateGlobalY(float phi)
{
    glm::vec4 axis = glm::vec4(0.0f,1.0f,0.0f,0.0f) * appliedTransform;
    rotationMatrix *= mop::Matrix_Rotate(phi, axis);
}

void Node3D::rotateGlobalZ(float phi)
{
    glm::vec4 axis = glm::vec4(0.0f,0.0f,1.0f,0.0f) * appliedTransform;
    rotationMatrix *= mop::Matrix_Rotate(phi, axis);
}

void Node3D::translate(glm::vec3 translation)
{
    positionMatrix *= mop::Matrix_Translate(translation.x, translation.y, translation.z);
}

void Node3D::resetRotation()
{
    rotationMatrix = mop::Matrix_Identity();
}

void Node3D::globalTranslate(glm::vec3 translation)
{
    translation = appliedTransform * glm::vec4(translation, 0.0);
    positionMatrix[3][0] += translation.x;
    positionMatrix[3][1] += translation.y;
    positionMatrix[3][2] += translation.z;
}

void Node3D::localTranslateX(float distance)
{
    glm::vec3 axisBasis(getBasisX());
    translate(axisBasis * distance);
}

void Node3D::localTranslateY(float distance)
{
    glm::vec3 axisBasis(getBasisY());
    translate(axisBasis * distance);
}

void Node3D::localTranslateZ(float distance)
{
    glm::vec3 axisBasis(getBasisZ());
    translate(axisBasis * distance);
}

void Node3D::scale(glm::vec3 scaleAmount)
{
    scaleMatrix *= mop::Matrix_Scale(scaleAmount.x, scaleAmount.y, scaleAmount.z);
}

void Node3D::setPosition(glm::vec3 position)
{
    positionMatrix[3][0] = position.x;
    positionMatrix[3][1] = position.y;
    positionMatrix[3][2] = position.z;
}

void Node3D::setGlobalPosition(glm::vec3 position)
{
    glm::vec4 newPos = glm::vec4(position, 1.0f) * appliedTransform;

    positionMatrix[3][0] = newPos.x;
    positionMatrix[3][1] = newPos.y;
    positionMatrix[3][2] = newPos.z;
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
    return glm::vec4(appliedTransform[3]);
}

void Node3D::setAppliedTransform(glm::mat4 newTransform)
{
    appliedTransform = newTransform;
}
