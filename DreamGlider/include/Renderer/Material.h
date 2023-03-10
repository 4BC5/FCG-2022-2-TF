#ifndef MATERIAL_H
#define MATERIAL_H
#include <string>

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <iostream>
#include <Texture.h>

const std::string TEX_WHITE_PATH = "../DreamGliderAssets/Materials/Default/white.jpg";
const std::string TEX_DEFAULT_NORMAL_PATH = "../DreamGliderAssets/Materials/Default/norm.jpg";

class Material
{
    public:
        Material(glm::vec4 color, Texture* albedoTexture, Texture* normalMap, Texture* ormMap);
        Material(Texture* albedoTexture, Texture* normalMap, Texture* ormMap);
        Material(Texture* albedoTexture, Texture* normalMap);
        Material(Texture* albedoTexture);
        Material(glm::vec4 color, Texture* albedoTexture);
        Material(glm::vec4 color);
        virtual ~Material();

        void sendEssentialTextures(GLuint program);
        void addExtraTextre(std::string uniformName, Texture* texture);
        void sendExtraTextures(GLuint program);
        void sendMaterialSettings(GLuint program);

        void setUVTiling(glm::vec2 UVtiling){this->UVtiling = UVtiling;}
        void setFaceCulling(bool culling){faceCulling = culling;}
        void setFaceCullingMode(GLenum cullingMode){faceCullingMode = cullingMode;}
        void setShaderType(int type){shaderType = type;}
        void setTransmission(float transmission){this->transmission = transmission;}
        void setNormalStrength(float normalStrength){this->normalStrength = normalStrength;}
        void setColor(glm::vec4 color){this->color = color;}
        void setSpecularPower(float specularPower){this->specularPower = specularPower;}
        void setSpecularStrength(float specularStrength){this->specularStrength = specularStrength;}
        void setMetallic(float metallic){this->metallic = metallic;}
        void setRoughness(float roughness){this->roughness = roughness;}

        int getShaderType(){return shaderType;}
        bool getFaceCulling(){return faceCulling;}
        GLenum getFaceCullingMode(){return faceCullingMode;}
        Texture* getAlbedoTexture(){return albedoTexture;}
        glm::vec2 getUVTiling(){return UVtiling;}

        static void initializeDefaultTextures();
        static Texture* whiteTexture;
        static Texture* defaultNormal;

    protected:
    private:
        int shaderType = 0;
        glm::vec4 color = glm::vec4(1.0);

        Texture* albedoTexture;
        Texture* normalTexture;
        Texture* ormTexture;
        std::vector<Texture*> extraTextures;
        std::vector<std::string> extraTexturesUniformNames;

        bool faceCulling = true;
        GLenum faceCullingMode = GL_BACK;
        glm::vec2 UVtiling = glm::vec2(1.0f);

        float transmission = 0.0;
        float normalStrength = 1.0;
        float specularPower = 1.0;
        float specularStrength = 0.5;

        float roughness = 1.0;
        float metallic = 0.0;

        int extraTextureCount = 0;
        void resetTextureIndices();
};


#endif // MATERIAL_H
