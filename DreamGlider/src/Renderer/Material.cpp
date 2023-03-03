#include "Material.h"

void Material::resetTextureIndices()
{
    albedoTexIndex = 0;
    normalTexIndex = 0;
    roughnessTextureIndex = 0;
    shaderType = 0;
}

Material::Material(glm::vec4 color, std::string albedoTexture, std::string normalMap, std::string roughnessMap)
{
    this->color = color;
    this->albedoTexturePath = albedoTexture;
    this->normalMapPath = normalMap;
    this->roughnessMapPath = roughnessMap;
    resetTextureIndices();
}

Material::Material(std::string albedoTexture, std::string normalMap, std::string roughnessMap)
{
    this->color = glm::vec4(1.0f);
    this->albedoTexturePath = albedoTexture;
    this->normalMapPath = normalMap;
    this->roughnessMapPath = roughnessMap;
    resetTextureIndices();
}

Material::Material(std::string albedoTexture)
{
    this->color = glm::vec4(1.0f);
    this->albedoTexturePath = albedoTexture;
    this->normalMapPath = TEX_DEFAULT_NORMAL_PATH;
    this->roughnessMapPath = TEX_WHITE_PATH;
    resetTextureIndices();
}

Material::Material(glm::vec4 color, std::string albedoTexture)
{
    this->color = glm::vec4(1.0f);
    this->albedoTexturePath = albedoTexture;
    this->normalMapPath = TEX_DEFAULT_NORMAL_PATH;
    this->roughnessMapPath = TEX_WHITE_PATH;
    resetTextureIndices();
}

Material::Material(glm::vec4 color)
{
    this->color = color;
    this->albedoTexturePath = TEX_WHITE_PATH;
    this->normalMapPath = TEX_DEFAULT_NORMAL_PATH;
    this->roughnessMapPath = TEX_WHITE_PATH;
    resetTextureIndices();
}

Material::Material(std::string albedoTexture, std::string normalMap)
{
    this->color = color;
    this->albedoTexturePath = albedoTexture;
    this->normalMapPath = normalMap;
    this->roughnessMapPath = TEX_WHITE_PATH;
    resetTextureIndices();
}
Material::~Material()
{
    //dtor
}
