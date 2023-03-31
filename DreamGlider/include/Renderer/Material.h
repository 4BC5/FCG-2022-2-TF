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

const std::string TEX_WHITE_PATH = "../DreamGliderAssets/Materials/Default/white.jpg";//Caminho de textura branca
const std::string TEX_DEFAULT_NORMAL_PATH = "../DreamGliderAssets/Materials/Default/norm.jpg";//Caminho de normal map básico

//Define os materiais a serem usados para renderização

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

        void sendEssentialTextures(GLuint program);//Manda as texturas básicas para a GPU
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
        int shaderType = 0;//Tipo de shader a usar
        glm::vec4 color = glm::vec4(1.0);//Cor do material

        Texture* albedoTexture;//Textura de cor básica
        Texture* normalTexture;//Textura para normal mapping
        Texture* ormTexture;//Textura ORM (ambient occlusion, roughness, metalness)

        Texture* secondaryAlbedoTexture = nullptr;//Albedo secundário para terrenos
        Texture* secondaryNormalTexture = nullptr;//Normal secundário para terrenos
        Texture* secondaryOrmTexture = nullptr;//ORM secundário para terrenos

        Texture* terrainMap = nullptr;//Texura que mapeia partes do terreno

        std::vector<Texture*> extraTextures;//Texturas extra (não utilizado)
        std::vector<std::string> extraTexturesUniformNames;

        mutable std::unordered_map<std::string, GLint> uniformLocationCache;//Cache de uniforms, para não precisar ficar perguntando o local para a GPU

        bool faceCulling = true;//Se o material usa face culling
        GLenum faceCullingMode = GL_BACK;//O tipo de culling que o material usa
        glm::vec2 UVtiling = glm::vec2(1.0f);//Multiplicador das UVs, para repetir texturas
        glm::vec2 UVScrolling = glm::vec2(0.0f);//Scrolling das UVs, para animar texturas

        bool transparent = false;//Se o material é transparente (translúcido)
        float transmission = 0.0;//Transmissão de luz através do material
        float normalStrength = 1.0;//Força no normal mapping
        float specularPower = 1.0;//Specular de Blinn-Phong
        float specularStrength = 0.5;//Multiplicador do especular de Blinn-Phong

        float roughness = 1.0;//O quão ásperas são as microfáces é a superfície
        float metallic = 0.0;//O quão metalica é a superfície

        int extraTextureCount = 0;//Número de texturas extra (não utilizado)
        void resetTextureIndices();

        bool terrainShader = false;//Se o shader é de terreno
};


#endif // MATERIAL_H
