//Implementação da Curva de Bézier -> Funções

#include <Nodes/Node.h>
#include <Nodes/Node3D.h>
#include <matrices.h>
#include <glm/mat4x4.hpp>
#include <sstream>

Curves::Curves(std::string name) : Node3D(name)
{
    type = 1;
    positionMatrix = mop::Matrix_Identity();
    rotationMatrix = mop::Matrix_Identity();
    scaleMatrix = mop::Matrix_Identity();
    appliedTransform = mop::Matrix_Identity();
}

Curves::~Curves()
{
    //dtor
}
/*
void Curves::applyGlobalTransform()
{
    appliedTransform = parent->getGlobalTransform() * positionMatrix * scaleMatrix * rotationMatrix;
}

void Curves::rotateAxis(glm::vec3 axis, float phi)
{
    rotationMatrix *= mop::Matrix_Rotate(phi, glm::vec4(axis.x,axis.y,axis.z,0.0f));
}

void Curves::rotateLocalX(float phi)
{
    rotationMatrix *= mop::Matrix_Rotate(phi, glm::vec4(transform[0][0],transform[0][1],transform[0][2],0.0f));
}

void Curves::rotateLocalY(float phi)
{
    rotationMatrix *= mop::Matrix_Rotate(phi, glm::vec4(transform[1][0],transform[1][1],transform[2][2],0.0f));
}

void Curves::rotateLocalZ(float phi)
{
    rotationMatrix *= mop::Matrix_Rotate(phi, glm::vec4(transform[2][0],transform[2][1],transform[2][2],0.0f));
}

void Curves::rotateGlobalX(float phi)
{
    rotationMatrix *= mop::Matrix_Rotate_X(phi);
}

void Curves::rotateGlobalY(float phi)
{
    //mop::PrintMatrix(getGlobalTransform());
    rotationMatrix *= mop::Matrix_Rotate_Y(phi);
}

void Curves::rotateGlobalZ(float phi)
{
    rotationMatrix *= mop::Matrix_Rotate_Z(phi);
}

void Curves::translate(glm::vec3 translation)
{
    positionMatrix *= mop::Matrix_Translate(translation.x, translation.y, translation.z);
}

void Curves::globalTranslate(glm::vec3 translation)
{
    positionMatrix = positionMatrix * parent->getGlobalTransform();
    positionMatrix[3][0] += translation.x;
    positionMatrix[3][1] += translation.y;
    positionMatrix[3][2] += translation.z;
    //mop::PrintMatrix(positionMatrix);
    //positionMatrix = glm::inverse(parent->getGlobalTransform()) * appliedTransform;
}

void Curves::scale(glm::vec3 scaleAmount)
{
    scaleMatrix *= mop::Matrix_Scale(scaleAmount.x, scaleAmount.y, scaleAmount.z);
}

glm::mat4 Curves::getTransform()
{
    return scaleMatrix * rotationMatrix * positionMatrix;
}

glm::vec4 Curves::getPosition()
{
    return glm::vec4(positionMatrix[3][0], positionMatrix[3][1], positionMatrix[3][2], 1.0f);
}

glm::vec4 Curves::getBasisX()
{
    glm::mat4 basis = scaleMatrix * rotationMatrix;
    return glm::vec4(basis[0][0],basis[0][1],basis[0][2], 0.0f);
}

glm::vec4 Curves::getBasisY()
{
    glm::mat4 basis = scaleMatrix * rotationMatrix;
    return glm::vec4(basis[1][0],basis[1][1],basis[1][2], 0.0f);
}

glm::vec4 Curves::getBasisZ()
{
    glm::mat4 basis = scaleMatrix * rotationMatrix;
    return glm::vec4(basis[2][0],basis[2][1],basis[2][2], 0.0f);
}

glm::vec4 Curves::getGlobalBasisX()
{
    return glm::vec4(appliedTransform[0][0],appliedTransform[0][1],appliedTransform[0][2], 0.0f);
}

glm::vec4 Curves::getGlobalBasisY()
{
    return glm::vec4(appliedTransform[1][0],appliedTransform[1][1],appliedTransform[1][2], 0.0f);
}

glm::vec4 Curves::getGlobalBasisZ()
{
    return glm::vec4(appliedTransform[2][0],appliedTransform[2][1],appliedTransform[2][2], 0.0f);
}

glm::mat4 Curves::getGlobalTransform()
{
    return appliedTransform;
}

glm::vec4 Curves::getGlobalPosition()
{
    return glm::vec4(appliedTransform[3][0], appliedTransform[3][1], appliedTransform[3][2], appliedTransform[3][3]);
}

void Curves::setAppliedTransform(glm::mat4 newTransform)
{
    appliedTransform = newTransform;
}*/
