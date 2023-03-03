#include "Camera.h"
#include <iostream>

Camera::Camera(std::string name, float planeNear, float planeFar, float fov) : Node3D(name)
{
    nearClippingPlane = planeNear;
    farClippingPlane = planeFar;
}

Camera::~Camera()
{
    //dtor
}

glm::mat4 Camera::getCameraMatrix()
{
    return mop::Matrix_Camera_View(getGlobalPosition(),-getGlobalBasisZ(),getGlobalBasisY());
}

glm::mat4 Camera::getProjectionMatrix(float screenRatio)
{
    //return getOrthoMatrix(-screenRatio,screenRatio,-screenRatio,screenRatio);
    return getPerspectiveMatrix(screenRatio);
}

glm::mat4 Camera::getPerspectiveMatrix(float screenRatio)
{
    return mop::Matrix_Perspective(fov, screenRatio, -nearClippingPlane, -farClippingPlane);
}

glm::mat4 Camera::getOrthoMatrix(float l, float r, float b, float t)
{
    return mop::Matrix_Orthographic(l, r, b, t, -nearClippingPlane, -farClippingPlane);
}

glm::mat4 Camera::getLightSpaceMatrix(float squareSize)
{
    glm::mat4 ortho = getOrthoMatrix(-squareSize, squareSize, -squareSize, squareSize);
    glm::mat4 view = getCameraMatrix();//glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
    return ortho * view;
}
