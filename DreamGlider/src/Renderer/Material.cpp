#include "Material.h"
#include <utils.h>

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

GLint Material::getUniformLocation(GLuint program, const std::string& name) const //Faz caching de uniform para poupar queries da GPU, FONTE técnica de https://youtu.be/nBB0LGSIm5Q
{
    auto searchLoc = uniformLocationCache.find(name);
    if (searchLoc != uniformLocationCache.end())
        return searchLoc->second;

    GLint location = glGetUniformLocation(program, name.c_str());
    uniformLocationCache[name] = location;
    return location;
}

void Material::sendMaterialSettings(GLuint program)
{
    GLint UVScaleUniform = getUniformLocation(program, "UVTiling");
    GLint transmissionUniform = getUniformLocation(program, "transmission");
    GLint normalStrengthUniform = getUniformLocation(program, "normalStrength");
    GLint colorUniform = getUniformLocation(program, "color");
    GLint specularPowerUniform = getUniformLocation(program, "specularPower");
    GLint specularStrengthUniform = getUniformLocation(program, "specularStrength");

    GLint metallicUniform = getUniformLocation(program, "metallicMultiplier");
    GLint roughnessUniform = getUniformLocation(program, "roughnessMultiplier");
    GLint UVscrollUniform = getUniformLocation(program, "UVScrolling");

    glUniform2f(UVscrollUniform, UVScrolling.x, UVScrolling.y);
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
    GLint albedoUniform = getUniformLocation(program, "albedoTexture");
    GLint normalUniform = getUniformLocation(program, "normalTexture");
    GLint ormUniform = getUniformLocation(program, "ORMTexture");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,albedoTexture->getTextureId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,normalTexture->getTextureId());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,ormTexture->getTextureId());

    glUniform1i(albedoUniform, 0);
    glUniform1i(normalUniform, 1);
    glUniform1i(ormUniform, 2);

    glCheckError();
    if (terrainMap)
    {
        GLint secondAlbedoUniform = getUniformLocation(program, "secondAlbedo");
        GLint secondNormalUniform = getUniformLocation(program, "secondNormal");
        GLint secondOrmUniform = getUniformLocation(program, "secondORM");
        GLint terrainMapUniform = getUniformLocation(program, "terrainMap");
        //////////////////// S ALBEDO
        if (secondaryAlbedoTexture != nullptr)
        {
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D,secondaryAlbedoTexture->getTextureId());
            glUniform1i(secondAlbedoUniform, 4);
        }
        else
        {
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D,whiteTexture->getTextureId());
            glUniform1i(secondAlbedoUniform, 4);
        }

        //////////////////// S NORMAL
        if (secondaryNormalTexture != nullptr)
        {
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D,secondaryNormalTexture->getTextureId());
            glUniform1i(secondNormalUniform, 5);
        }
        else
        {
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D,defaultNormal->getTextureId());
            glUniform1i(secondNormalUniform, 5);
        }

        //////////////////// S ORM
        if (secondaryOrmTexture != nullptr)
        {
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D,secondaryOrmTexture->getTextureId());
            glUniform1i(secondOrmUniform, 6);
        }
        else
        {
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D,whiteTexture->getTextureId());
            glUniform1i(secondOrmUniform, 6);
        }

        //////////////////// S TERRAIN
        if (terrainMap != nullptr)
        {
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D,terrainMap->getTextureId());
            glUniform1i(terrainMapUniform, 7);
        }
        else
        {
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D,whiteTexture->getTextureId());
            glUniform1i(terrainMapUniform, 7);
        }
    }
    glCheckError();
}
#include <Renderer.h>

void Material::setShaderType(int type)
{
    shaderType = type;
    terrainShader = shaderType == SHADER_TERRAIN;

}
