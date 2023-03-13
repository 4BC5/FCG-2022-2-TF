#include "Camera.h"
#include <iostream>
#include <cmath>
#include <SceneManager.h>

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

/*void Camera::applyGlobalTransform()
{
    float currentY = getGlobalPosition().y;
    if (getParent()->type == NODE_TYPE_NODE_3D)
    {
        Node3D* parent3D = static_cast<Node3D*>(getParent());
        currentY = parent3D->getGlobalPosition().y;
    }

    if (std::abs(getGlobalPosition().y - currentY) > 0.2)
    {
        setPosY(0.0f);
    }

    float diff = prevY - currentY;
    translate(glm::vec3(0.0f, diff, 0.0f));

    float f = SceneManager::getDeltaTime() * std::min(std::max(std::abs(diff)/0.2f, 0.0f),1.0f) * 60.0f;
    setPosY(getPosition().y * (1.0 - f));


    if (getParent()->type == NODE_TYPE_NODE_3D)
    {
        Node3D* parent3D = static_cast<Node3D*>(getParent());
        prevY = parent3D->getGlobalPosition().y;
    }
    Node3D::applyGlobalTransform();
}*/
