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

    void sendLightMatrix(GLuint uniformLocation, int index);
    void sendShadowTextures(GLuint program);
    void bindShadowFBO(unsigned int index);
    void setShadowsEnabled(bool enabled);
    void setUpLightMatrices(Camera* camera, Window* window);

    void setNumShadowSamples(int shadowSamples){this->shadowSamples = shadowSamples;}
    void setShadowBias(float bias){shadowBias = bias;}
    void setCascadeBiasMultiplier(float multiplier){biasSplitMultiplier = multiplier;}
    void setColor(glm::vec4 lightColor){this->lightColor = lightColor;}
    void setIntensity(float intensity){this->intensity = intensity;}
    void setShadowBlur(float shadowBlur){this->shadowBlur = shadowBlur;}
    void setBiasSplitMultiplier(float biasSplitMultiplier){this->biasSplitMultiplier = biasSplitMultiplier;}

    glm::mat4* getLightMatrices(){return lightSpaceMatrices;}
    GLuint* getShadowmapTextures(){return shadowMapTextures;}

    bool getShadowsEnabled(){return shadowsEnabled;}
    float getShadowResolution(){return shadowResolution;}
    float getShadowBias(){return shadowBias;}
    //float getCascadeBiasMultiplier(){return biasSplitMultiplier;}
    int getCascadeCount(){return cascadeCount;}
    int getNumShadowSamples(){return shadowSamples;}
    glm::vec4 getLightDirection(){return -getGlobalBasisZ();}
    float getIntensity(){return intensity;}
    glm::vec4 getLightColor(){return lightColor;}
    float* getCascadeDistances(){return cascadeDistances;}
    float getBiasSplitMultiplier(){return biasSplitMultiplier;}
    float getShadowBlur(){return shadowBlur;}


    //void DirectionalLight::applyGlobalTransform()
    protected:

    private:
        glm::vec4 lightColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);
        float intensity = 1.0f;

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
        GLfloat cascadeDistances[4] = {10.0f, 30.0f, 140.0f, 400.0f};
        glm::mat4 lightSpaceMatrices[4];

        int shadowSamples = 12;
        float shadowBias = 0.0025;
        float biasSplitMultiplier = 2.0f;
        float shadowBlur = 0.5;
};

#endif // DIRECTIONALLIGHT_H
