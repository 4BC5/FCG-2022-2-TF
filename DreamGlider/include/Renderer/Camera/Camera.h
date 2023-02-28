#ifndef CAMERA_H
#define CAMERA_H
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <matrices.h>
#include <Nodes/Node3D.h>


class Camera : public Node3D
{
    public:
        Camera();
        virtual ~Camera();
        glm::vec4 upVector = glm::vec4(0.0f,1.0f,0.0f,0.0f);

        glm::mat4 getCameraMatrix();
        glm::mat4 getProjectionMatrix();
        float getFOV(){return fov;}
        float getNearPlane(){return nearPlane;}
        float getFarPlane(){return farPlane;}

    protected:

    private:
        float fov = 3.141592f/2.0f;
        float nearPlane = 0.25f;
        float farPlane = 2000.0f;
};

#endif // CAMERA_H
