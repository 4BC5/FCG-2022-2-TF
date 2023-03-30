#ifndef MATERIAL_H
#define MATERIAL_H
#include <string>

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <iostream>
#include <Texture.h>
#include <unordered_map>

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
        void setUVScrolling(glm::vec2 UVScrolling){this->UVScrolling = UVScrolling;}
        void setFaceCulling(bool culling){faceCulling = culling;}
        void setFaceCullingMode(GLenum cullingMode){faceCullingMode = cullingMode;}
        void setShaderType(int type);
        void setTransmission(float transmission){this->transmission = transmission;}
        void setNormalStrength(float normalStrength){this->normalStrength = normalStrength;}
        void setColor(glm::vec4 color){this->color = color;}
        void setSpecularPower(float specularPower){this->specularPower = specularPower;}
        void setSpecularStrength(float specularStrength){this->specularStrength = specularStrength;}
        void setMetallic(float metallic){this->metallic = metallic;}
        void setRoughness(float roughness){this->roughness = roughness;}
        void setTransparent(bool transparent){this->transparent = transparent;}

        void setAlbedoTexture(Texture* albedoTexture){this->albedoTexture = albedoTexture;}
        void setNormalTexture(Texture* normalTexture){this->normalTexture = normalTexture;}
        void setOrmTexture(Texture* ormTexture){this->ormTexture = ormTexture;}

        void setSecondaryAlbedoTexture(Texture* nAlbedo){this->secondaryAlbedoTexture = nAlbedo;}
        void setSecondaryNormalTexture(Texture* nNormal){this->secondaryNormalTexture = nNormal;}
        void setSecondaryOrmTexture(Texture* nORM){this->secondaryOrmTexture = nORM;}

        void setTerrainMap(Texture* terrainMap){this->terrainMap = terrainMap;}

        int getShaderType(){return shaderType;}
        bool getFaceCulling(){return faceCulling;}
        bool getTransparent(){return transparent;}
        GLenum getFaceCullingMode(){return faceCullingMode;}
        Texture* getAlbedoTexture(){return albedoTexture;}
        glm::vec2 getUVTiling(){return UVtiling;}

        static void initializeDefaultTextures();
        static Texture* whiteTexture;
        static Texture* defaultNormal;

        GLint getUniformLocation(GLuint program, const std::string& name) const;

    protected:

    private:
        int shaderType = 0;
        glm::vec4 color = glm::vec4(1.0);

        Texture* albedoTexture;
        Texture* normalTexture;
        Texture* ormTexture;

        Texture* secondaryAlbedoTexture = nullptr;
        Texture* secondaryNormalTexture = nullptr;
        Texture* secondaryOrmTexture = nullptr;

        Texture* terrainMap = nullptr;

        std::vector<Texture*> extraTextures;
        std::vector<std::string> extraTexturesUniformNames;
        mutable std::unordered_map<std::string, GLint> uniformLocationCache;

        bool faceCulling = true;
        GLenum faceCullingMode = GL_BACK;
        glm::vec2 UVtiling = glm::vec2(1.0f);
        glm::vec2 UVScrolling = glm::vec2(0.0f);

        bool transparent = false;
        float transmission = 0.0;
        float normalStrength = 1.0;
        float specularPower = 1.0;
        float specularStrength = 0.5;

        float roughness = 1.0;
        float metallic = 0.0;

        int extraTextureCount = 0;
        void resetTextureIndices();

        bool terrainShader = false;
};


#endif // MATERIAL_H
