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

    void sendShadowTextures(GLuint uniformLocation);
    void sendLightMatrices(GLuint uniformLocation);
    void sendLightMatrix(GLuint uniformLocation, int index);
    void sendCascadeClipEnds(GLuint uniformLocation);
    void sendLightDirection(GLuint uniformLocation);
    void sendCascadeCount(GLuint uniformLocation);
    void bindShadowFBO(unsigned int index);
    void setShadowsEnabled(bool enabled);
    void setUpLightMatrices(Camera* camera, Window* window);

    bool getShadowsEnabled(){return shadowsEnabled;}
    float getShadowResolution(){return shadowResolution;}
    int getCascadeCount(){return cascadeCount;}
    glm::vec4 getLightDirection(){return -getGlobalBasisZ();}


    //void DirectionalLight::applyGlobalTransform()
    protected:

    private:
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
        GLfloat cascadeDistances[4] = {20.0f, 40.0f, 70.0f, 300.0f};
        GLfloat cascadeClipEnds[4];
        glm::mat4 lightSpaceMatrices[4];
};

#endif // DIRECTIONALLIGHT_H
