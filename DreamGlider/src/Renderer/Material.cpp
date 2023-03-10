#include "Material.h"

Texture* Material::whiteTexture = 0;
Texture* Material::defaultNormal = 0;

void Material::initializeDefaultTextures()
{
    Material::whiteTexture = new Texture(TEX_WHITE_PATH);
    Material::defaultNormal = new Texture(TEX_DEFAULT_NORMAL_PATH);
}

void Material::resetTextureIndices()
{
    shaderType = 0;
}

Material::Material(glm::vec4 color, Texture* albedoTexture, Texture* normalMap, Texture* ormMap)
{
    this->color = color;
    this->albedoTexture = albedoTexture;
    this->normalTexture = normalMap;
    this->ormTexture = ormMap;
    resetTextureIndices();
}

Material::Material(Texture* albedoTexture, Texture* normalMap, Texture* ormMap)
{
    this->color = glm::vec4(1.0f);
    this->albedoTexture = albedoTexture;
    this->normalTexture = normalMap;
    this->ormTexture = ormMap;
    resetTextureIndices();
}

Material::Material(Texture* albedoTexture)
{
    this->color = glm::vec4(1.0f);
    this->albedoTexture = albedoTexture;
    this->normalTexture = Material::defaultNormal;
    this->ormTexture = Material::whiteTexture;
    resetTextureIndices();
}

Material::Material(glm::vec4 color, Texture* albedoTexture)
{
    this->color = glm::vec4(1.0f);
    this->albedoTexture = albedoTexture;
    this->normalTexture = Material::defaultNormal;
    this->ormTexture = Material::whiteTexture;
    resetTextureIndices();
}

Material::Material(glm::vec4 color)
{
    this->color = color;
    this->albedoTexture = Material::whiteTexture;
    this->normalTexture = Material::defaultNormal;
    this->ormTexture = whiteTexture;
    resetTextureIndices();
}

Material::Material(Texture* albedoTexture, Texture* normalMap)
{
    this->color = color;
    this->albedoTexture = albedoTexture;
    this->normalTexture = normalMap;
    this->ormTexture = Material::whiteTexture;
    resetTextureIndices();
}
Material::~Material()
{
    //dtor
}

void Material::addExtraTextre(std::string uniformName, Texture* texture)
{
    if (extraTextureCount > 2)
    {
        std::cout << "Can't have more than 3 extra textures\n";
        return;
    }
    extraTextureCount += 1;
    extraTextures.push_back(texture);
    extraTexturesUniformNames.push_back(uniformName);
}

void Material::sendExtraTextures(GLuint program)
{
    for (unsigned int i = 0; i < extraTextureCount; i++)
    {
        GLint uniformLocation = glGetUniformLocation(program, extraTexturesUniformNames[i].c_str());
        glActiveTexture(GL_TEXTURE4 + i);
        glBindTexture(GL_TEXTURE_2D, extraTextures[i]->getTextureId());
        glUniform1i(uniformLocation, i + 4);
    }
}

void Material::sendMaterialSettings(GLuint program)
{
    GLint UVScaleUniform = glGetUniformLocation(program, "UVTiling");
    GLint transmissionUniform = glGetUniformLocation(program, "transmission");
    GLint normalStrengthUniform = glGetUniformLocation(program, "normalStrength");
    GLint colorUniform = glGetUniformLocation(program, "color");
    GLint specularPowerUniform = glGetUniformLocation(program, "specularPower");
    GLint specularStrengthUniform = glGetUniformLocation(program, "specularStrength");

    GLint metallicUniform = glGetUniformLocation(program, "metallicMultiplier");
    GLint roughnessUniform = glGetUniformLocation(program, "roughnessMultiplier");

    glUniform2f(UVScaleUniform, UVtiling.x, UVtiling.y);
    glUniform1f(transmissionUniform, transmission);
    glUniform1f(normalStrengthUniform, normalStrength);
    glUniform1f(specularPowerUniform, specularPower);
    glUniform1f(specularStrengthUniform, specularStrength);
    glUniform4f(colorUniform, color.x, color.y, color.z, color.w);
    glUniform1f(metallicUniform, metallic);
    glUniform1f(roughnessUniform, roughness);
}

void Material::sendEssentialTextures(GLuint program)
{
    GLint albedoUniform = glGetUniformLocation(program, "albedoTexture");
    GLint normalUniform = glGetUniformLocation(program, "normalTexture");
    GLint ormUniform = glGetUniformLocation(program, "ORMTexture");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,albedoTexture->getTextureId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,normalTexture->getTextureId());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,ormTexture->getTextureId());

    glUniform1i(albedoUniform, 0);
    glUniform1i(normalUniform, 1);
    glUniform1i(ormUniform, 2);
}
