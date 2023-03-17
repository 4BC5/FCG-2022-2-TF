#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include <Renderer/Camera/Camera.h>
#include <iostream>
#include <utils.h>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <Window/Window.h>


class DirectionalLight : public Camera
{
    public:
        DirectionalLight(std::string name);
        virtual ~DirectionalLight();

    void applyGlobalTransform();

    void setShadowResolution(int resolution);
    void setShadowsOn(bool shadowsOn);

    void sendLightSettings(GLuint program);
    void sendShadowTextures(GLuint uniformLocation);
    void sendLightMatrices(GLuint uniformLocation);
    void sendLightMatrix(GLuint uniformLocation, int index);
    void sendCascadeClipEnds(GLuint uniformLocation);
    void sendCascadeCount(GLuint uniformLocation);
    void sendShadowSettings(GLuint program);
    void bindShadowFBO(unsigned int index);
    void setShadowsEnabled(bool enabled);
    void setUpLightMatrices(Camera* camera, Window* window);

    void setNumShadowSamples(int shadowSamples){this->shadowSamples = shadowSamples;}
    void setShadowBias(float bias){shadowBias = bias;}
    void setCascadeBiasMultiplier(float multiplier){cascadeShadowBiasMultiplier = multiplier;}
    void setSunColor(glm::vec4 sunColor){this->sunColor = sunColor;}
    void setSunIntensity(float sunIntensity){this->sunIntensity = sunIntensity;}
    void setShadowBlur(float shadowBlur){this->shadowBlur = shadowBlur;}

    bool getShadowsEnabled(){return shadowsEnabled;}
    float getShadowResolution(){return shadowResolution;}
    float getShadowBias(){return shadowBias;}
    float getCascadeBiasMultiplier(){return cascadeShadowBiasMultiplier;}
    int getCascadeCount(){return cascadeCount;}
    int getNumShadowSamples(){return shadowSamples;}
    glm::vec4 getLightDirection(){return -getGlobalBasisZ();}


    //void DirectionalLight::applyGlobalTransform()
    protected:

    private:
        glm::vec4 sunColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);
        float sunIntensity = 1.0f;

        float XRot = 3.141592f/2.0f;
        float YRot = 0.0f;

        void setUpShadowMaps();
        void deleteShadowMaps();
        glm::mat4 getLightMatrix(Camera* camera, Window* window, float nearPlane, float farPlane);

        bool shadowsEnabled = false;
        int shadowResolution = 2048;
        GLsizei cascadeCount = 4;
        GLuint shadowMapFBO = 0;
        GLuint shadowMapTextures[4] = {0,0,0,0};
        GLfloat cascadeDistances[4] = {10.0f, 30.0f, 80.0f, 300.0f};
        GLfloat cascadeClipEnds[4];
        glm::mat4 lightSpaceMatrices[4];

        int shadowSamples = 12;
        float shadowBias = 0.0025;
        float cascadeShadowBiasMultiplier = 1.6;
        float shadowBlur = 0.5;
};

#endif // DIRECTIONALLIGHT_H
