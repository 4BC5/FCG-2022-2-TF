#ifndef CAMERA_H
#define CAMERA_H
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <matrices.h>
#include <Nodes/Node3D.h>


class Camera : public Node3D
{
    public:
        Camera(std::string name, float planeNear, float planeFar, float fov);
        virtual ~Camera();
        glm::vec4 upVector = glm::vec4(0.0f,1.0f,0.0f,0.0f);

        glm::mat4 getCameraMatrix();
        glm::mat4 getProjectionMatrix(float screenRatio);
        glm::mat4 getPerspectiveMatrix(float screenRatio);
        glm::mat4 getOrthoMatrix(float l, float r, float b, float t);

        float getFOV(){return fov;}
        float getNearPlane(){return nearClippingPlane;}
        float getFarPlane(){return farClippingPlane;}

        void setFov(float fov){this->fov = fov;}
        void setFar(float dist){nearClippingPlane = dist;}
        void setNear(float dist){nearClippingPlane = dist;}
        /*void setSmoothY(bool SmoothY){this->SmoothY = SmoothY;}

        void applyGlobalTransform();*/

        void setProjectionMode(bool ortho)
        {
            this->ortho = ortho;
        }

    protected:

    private:
        float nearClippingPlane;
        float farClippingPlane;
        bool ortho = false;
        float fov = 3.141592f/2.0f;
        /*bool SmoothY = false;
        float prevY = 0.0f;*/
};

#endif // CAMERA_H
