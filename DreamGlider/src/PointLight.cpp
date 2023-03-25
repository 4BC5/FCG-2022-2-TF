#include "PointLight.h"
#include <SceneManager.h>
#include <glm/gtc/type_ptr.hpp>

PointLight::PointLight(std::string name) : Node3D(name)
{
    type = NODE_TYPE_POINT_LIGHT;
    sceneManager->registerPointLight(this);
    setAABB(AABB(attenuationRadius, attenuationRadius, attenuationRadius));
}

PointLight::~PointLight()
{
    sceneManager->unregisterPointLight(this);
}

void PointLight::sendLightInfo(GLuint program, Material* mat, int index)
{
    std::string indexString = "pointLights[" + std::to_string(index) + "]";
    std::string posUniformName = indexString + ".position";
    std::string colUniformName = indexString + ".color";
    std::string intensityUniformName = indexString + ".intensity";
    std::string attenuationUniformName = indexString + ".attenuationRadius";

    GLint posUniform = mat->getUniformLocation(program, posUniformName);
    GLint colUniform = mat->getUniformLocation(program, colUniformName);
    GLint intensityUniform = mat->getUniformLocation(program, intensityUniformName);
    GLint attenuationUniform = mat->getUniformLocation(program, attenuationUniformName);

    glUniform4f(posUniform, getGlobalPosition().x,getGlobalPosition().y,getGlobalPosition().z,getGlobalPosition().w);
    glUniform4fv(colUniform, 1, glm::value_ptr(color));
    glUniform1fv(intensityUniform, 1, &intensity);
    glUniform1fv(attenuationUniform, 1, &attenuationRadius);
}

void PointLight::setAttenuationRadius(float attenuationRadius)
{
    this->attenuationRadius = attenuationRadius;
    aabb = AABB(attenuationRadius, attenuationRadius, attenuationRadius);
}
