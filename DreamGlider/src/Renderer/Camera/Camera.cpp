#include "Camera.h"

Camera::Camera()
{
    //ctor
}

Camera::~Camera()
{
    //dtor
}

glm::mat4 Camera::getCameraMatrix()
{
    return mop::Matrix_Camera_View(getPosition(),-getBasisZ(),upVector);
}

glm::mat4 Camera::getProjectionMatrix()
{
    float screenRatio = 16.0f/9.0f;
    return mop::Matrix_Perspective(fov, screenRatio, nearPlane, farPlane);
}
