#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include <Renderer/Camera/Camera.h>
#include <iostream>
#include <utils.h>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <Window/Window.h>

//Luz direcional
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
        glm::vec4 lightColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);//Cor da luz
        float intensity = 1.0f;//Intensidade da luz

        void setUpShadowMaps();//Faz o setup dos shadow maps (cria texturas e buffers)
        void deleteShadowMaps();//Deleta as texturas e buffers de shadow maps
        glm::mat4 getLightMatrix(Camera* camera, Window* window, float nearPlane, float farPlane);//Pega a matriz da visão da luz, no frustum da câmera e com certa distância

        bool shadowsEnabled = false;//Sombras ligadas
        int shadowResolution = 2048;//Resolução de cada cascata da sombra
        GLsizei cascadeCount = 4;//Número de cascatas de sombra
        GLuint shadowMapFBO = 0;//FBO do shadowmap
        GLuint shadowMapTextures[4] = {0,0,0,0};//Endereço dos shadow maps
        GLfloat cascadeDistances[4] = {10.0f, 40.0f, 140.0f, 600.0f};//Distância de cada cascata
        glm::mat4 lightSpaceMatrices[4];//Matrizes da visão da luz

        int shadowSamples = 12;//Quantos samples de PCF usar
        float shadowBias = 0.0065;//Bias das sombras para evitar Peter-Panning
        float biasSplitMultiplier = 2.0f;//Muliplicador de bias a cada cascata
        float shadowBlur = 0.5;//Distância que cada sample vai ser pego no shadow map
};

#endif // DIRECTIONALLIGHT_H
