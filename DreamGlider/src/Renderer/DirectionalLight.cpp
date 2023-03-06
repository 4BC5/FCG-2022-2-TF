#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(std::string name, Camera* cameraFollow, float distance) : Camera(name, 0.25f, distance * 2.0f, 2.0f)
{
    followDistance = distance;
    this->cameraFollow = cameraFollow;
}

DirectionalLight::~DirectionalLight()
{
    //dtor
}

void DirectionalLight::setUpShadowmap()
{
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowResolution, shadowResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f,1.0f,1.0f,1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalLight::setShadowsEnabled(bool shadowsEnabled)
{
    std::cout << "Shadows enabled: " << this->shadowsEnabled << "\n";
    if (this->shadowsEnabled == shadowsEnabled)
        return;
    this->shadowsEnabled = shadowsEnabled;
    if (shadowsEnabled)
    {
        std::cout << "Setting up shadowmaps\n";
        setUpShadowmap();
    }
    else
    {
        glDeleteFramebuffers(1, &depthMapFBO);
        depthMapFBO = 0;
        glDeleteTextures(1, &depthMap);
        depthMap = 0;
    }
}

void DirectionalLight::setShadowResolution(GLsizei resolution)
{
    if (shadowResolution == resolution)
        return;
    shadowResolution = resolution;
    if (shadowsEnabled)
    {
        glDeleteFramebuffers(1, &depthMapFBO);
        depthMapFBO = 0;
        glDeleteTextures(1, &depthMap);
        depthMap = 0;

        setUpShadowmap();
        glCheckError();
    }

}

void DirectionalLight::applyGlobalTransform()
{
    //Camera::applyGlobalTransform();
    glm::vec4 camPosition = cameraFollow->getGlobalPosition();
    setGlobalPosition(glm::vec3(camPosition.x,camPosition.y,camPosition.z));
    resetRotation();
    rotateGlobalY(YRot);
    rotateGlobalX(XRot);
    localTranslateZ(followDistance);
    appliedTransform = parent->getGlobalTransform() * positionMatrix * scaleMatrix * rotationMatrix;
}

std::vector<glm::vec4> DirectionalLight::getFrustumCornersWorldSpace(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
    glm::mat4 inversePVm = glm::inverse(projectionMatrix * viewMatrix);

    std::vector<glm::vec4> frustumCorners;

    for (unsigned int x = 0; x < 2; x++)
    {
        for (unsigned int y = 0; y < 2; y++)
        {
            for (unsigned int z = 0; z < 2; z++)
            {
                glm::vec4 pt =
                inversePVm * glm::vec4(
                        2.0f * x - 1.0f,
                        2.0f * y - 1.0f,
                        2.0f * z - 1.0f,
                        1.0f);
                frustumCorners.push_back(pt / pt.w);
            }
        }
    }
    return frustumCorners;
}
