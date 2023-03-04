#ifndef MATERIAL_H
#define MATERIAL_H
#include <string>

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

const std::string TEX_WHITE_PATH = "../DreamGliderAssets/Materials/Default/white.jpg";
const std::string TEX_DEFAULT_NORMAL_PATH = "../DreamGliderAssets/Materials/Default/norm.jpg";

class Material
{
    public:
        Material(glm::vec4 color, std::string albedoTexture, std::string normalMap, std::string roughnessMap);
        Material(std::string albedoTexture, std::string normalMap, std::string roughnessMap);
        Material(std::string albedoTexture, std::string normalMap);
        Material(std::string albedoTexture);
        Material(glm::vec4 color, std::string albedoTexture);
        Material(glm::vec4 color);
        virtual ~Material();

        int shaderType = 0;
        glm::vec4 color;
        std::string albedoTexturePath;
        std::string normalMapPath;
        std::string roughnessMapPath;
        GLuint albedoTexIndex;
        GLuint normalTexIndex;
        GLuint roughnessTextureIndex;

        bool faceCulling = true;
        GLenum faceCullingMode = GL_BACK;
        float normalStrength = 1.0;
        glm::vec2 UVtiling = glm::vec2(1.0f);

    glm::vec2 tiling;
    //std

    protected:

    private:
        void resetTextureIndices();
};

#endif // MATERIAL_H
