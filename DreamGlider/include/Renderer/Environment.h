#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include <glad/glad.h>
#include <string>
#include <stb_image.h>
#include <glm/mat4x4.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
//Classe que implementa skybox
class Environment
{
    public:
        Environment(std::string cubemapPath);
        virtual ~Environment();

        float getEnvironmentStrength(){return environmentStrength;}

        void setEnvironmentStrength(float environmentStrength){this->environmentStrength = environmentStrength;}
        void renderCube(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
        void sendCubemapTexture(GLuint program);


    protected:

    private:
        float environmentStrength = 1.2f;
        GLuint cubemapId = 0;
        GLuint cubeVAO = 0;
        GLuint program = 0;

        void buildCube();
        void loadCubemap(std::string cubemapPath);

        bool loadShader(std::string shaderPath, GLuint shaderID);
        GLuint loadVertexShader(std::string shaderPath);
        GLuint loadFragmentShader(std::string shaderPath);
        GLuint CreateGpuProgram(GLuint vertexShaderId, GLuint fragmentShaderId);
        GLuint loadGPUProgram(std::string path);
};

#endif // ENVIRONMENT_H
