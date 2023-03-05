#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include <Renderer/Camera/Camera.h>
#include <iostream>
#include <utils.h>

class DirectionalLight : public Camera
{
    public:
        DirectionalLight(std::string name, Camera* cameraFollow, float distance);
        virtual ~DirectionalLight();

        glm::vec4 getLightDirection(){return -getGlobalBasisZ();}
        glm::vec4 getLightColor(){return lightColor;}

        float getIntensity(){return intensity;}
        float getShadowRange(){return shadowRange;}
        bool getShadowsEnabled(){return shadowsEnabled;}

        GLuint getShadowmap(){return depthMap;}
        GLuint getShadowBuffer(){return depthMapFBO;}
        GLsizei getShadowResolution(){return shadowResolution;}

        std::vector<glm::vec4> getFrustumCornersWorldSpace(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);

        void setIntensity(float intensity){this->intensity = intensity;}
        void setLightColor(glm::vec4 lightColor){this->lightColor = lightColor;}
        void setShadowRange(float range){shadowRange = range;}

        void setShadowResolution(GLsizei resolution);
        void setShadowsEnabled(bool shadowsEnabled);
        void applyGlobalTransform() override;

    protected:

    private:
        void setUpShadowmap();
        void renderShadows();

        Camera* cameraFollow;
        float followDistance = 80.0;
        float YRot = 0.6f;
        float XRot = -3.141592f/1.4f;
        float shadowRange = 20.0f;
        GLsizei shadowResolution = 2048;
        bool shadowsEnabled = false;
        glm::vec4 lightColor = glm::vec4(1.0f);
        float intensity = 1.0f;
        GLuint depthMapFBO = 0;
        GLuint depthMap = 0;
};

#endif // DIRECTIONALLIGHT_H
