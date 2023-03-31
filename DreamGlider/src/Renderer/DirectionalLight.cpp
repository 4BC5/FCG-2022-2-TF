#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(std::string name) : Camera(name, 0.25f, 10.0f, 1.0f)
{
}

DirectionalLight::~DirectionalLight()
{
    //dtor
}

void DirectionalLight::setUpShadowMaps()
{
    deleteShadowMaps();

    glGenFramebuffers(1, &shadowMapFBO);

    glGenTextures(cascadeCount, shadowMapTextures);

    for (int i = 0; i < cascadeCount; i++)//Cria as texturas do shadow map, dependendo de quantas cascatas s�o desejadas
    {
        glBindTexture(GL_TEXTURE_2D, shadowMapTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, shadowResolution, shadowResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };//Cor de borda, para as sombras n�o se extenderem al�m da textura
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTextures[0], 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalLight::bindShadowFBO(unsigned int index)
{
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    if (index > 3)
        index = 3;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTextures[index], 0);

}

void DirectionalLight::setShadowsEnabled(bool enabled)
{
    if (enabled == shadowsEnabled)
        return;
    shadowsEnabled = true;
    if (enabled)
    {
        setUpShadowMaps();
    }
    else
    {
        deleteShadowMaps();
    }
}

void DirectionalLight::deleteShadowMaps()
{
    if (shadowMapFBO != 0)
    {
        glDeleteFramebuffers(1, &shadowMapFBO);
    }

    for (int i = 0; i < 4; i++)
    {
        if (shadowMapTextures[i] != 0)
        {
            glDeleteTextures(1, &shadowMapTextures[i]);
        }
    }
}

std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)//C�digo de https://learnopengl.com/Guest-Articles/2021/CSM e https://ogldev.org/www/tutorial49/tutorial49.html
{
    //Calcula os cantos do frustum usando a matriz inv�rsa de view e proje��o (de NDC para world)
    const auto inv = glm::inverse(proj * view);

    std::vector<glm::vec4> frustumCorners;
    for (unsigned int x = 0; x < 2; ++x)
    {
        for (unsigned int y = 0; y < 2; ++y)
        {
            for (unsigned int z = 0; z < 2; ++z)
            {
                const glm::vec4 pt =
                    inv * glm::vec4(
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

glm::mat4 DirectionalLight::getLightMatrix(Camera* camera, Window* window, float nearPlane, float farPlane)//C�digo adaptado de https://learnopengl.com/Guest-Articles/2021/CSM e https://ogldev.org/www/tutorial49/tutorial49.html
{
    //Esta fun��o calcula a matriz de view e proje��o da luz direcional, utilizando o frustum para fazer a proje��o cobrir a parte desejada (entre nearPlane e farPlane, que n�o s�o o near e far da c�mera, mas a se��o do frustum que a cascata cobre)
    const auto proj = mop::Matrix_Perspective(camera->getFOV(),window->getAspect(), nearPlane, farPlane);

    const auto corners = getFrustumCornersWorldSpace(proj, mop::Matrix_Camera_View(camera->getGlobalPosition(), camera->getGlobalBasisZ(), camera->getGlobalBasisY()));//Achar os cantos do
    glm::vec4 center = glm::vec4(0.0f,0.0f,0.0f,1.0f);
    for (const auto& v : corners)
    {
        center += v;
    }
    center /= corners.size();
    center.w = 1.0f;//Encontrar o centro do corte atual do frustum


    const auto lightView = mop::Matrix_Camera_View(center, -getLightDirection(), getGlobalBasisY());//View matrix da luz

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();
    for (const auto& v : corners)//Encontrar os limites da proje��o ortogr�fica baseado nos cantos do frustum
    {
        const auto trf = lightView * v;
        minX = std::min(minX, trf.x);
        maxX = std::max(maxX, trf.x);
        minY = std::min(minY, trf.y);
        maxY = std::max(maxY, trf.y);
        minZ = std::min(minZ, trf.z);
        maxZ = std::max(maxZ, trf.z);
    }
    float zMult = 40.0f;//Multiplicar a profundidade para evitar que objetos atr�s da c�mera sejam cortados
    if (minZ < 0)
    {
        minZ *= zMult;
    }
    else
    {
        minZ /= zMult;
    }
    if (maxZ < 0)
    {
            maxZ /= zMult;
    }
    else
    {
    maxZ *= zMult;
    }
    const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);//Matriz de proje��o da luz, com limites calculados anteriormente

    return lightProjection * lightView;
}

void DirectionalLight::setUpLightMatrices(Camera* camera, Window* window)
{
    lightSpaceMatrices[0] = getLightMatrix(camera, window, camera->getNearPlane(), cascadeDistances[0]);
    lightSpaceMatrices[1] = getLightMatrix(camera, window, cascadeDistances[0], cascadeDistances[1]);
    lightSpaceMatrices[2] = getLightMatrix(camera, window, cascadeDistances[1], cascadeDistances[2]);
    lightSpaceMatrices[3] = getLightMatrix(camera, window, cascadeDistances[2], cascadeDistances[3]);
}


void DirectionalLight::setShadowResolution(int resolution)
{
    if (shadowResolution == resolution)
        return;
    shadowResolution = resolution;
    if (shadowsEnabled)
    {
        setUpShadowMaps();
    }
}

/*void DirectionalLight::sendLightMatrices(GLuint uniformLocation)
{
    for (int i = 0; i < cascadeCount; i++)
    {
        glUniformMatrix4fv(uniformLocation, 4, GL_FALSE, glm::value_ptr(lightSpaceMatrices[0]));
    }
}*/

void DirectionalLight::sendShadowTextures(GLuint program)
{
    GLuint uniformLocation = glGetUniformLocation(program, "directionalShadowMap");
    for (int i = 0; i < cascadeCount; i++)
    {
        glActiveTexture(GL_TEXTURE12 + i);
        glBindTexture(GL_TEXTURE_2D, shadowMapTextures[i]);
    }
    GLint samplers[] = {12,13,14,15};
    glUniform1iv(uniformLocation, 4, samplers);
}

/*void DirectionalLight::sendLightSettings(GLuint program)
{
    GLuint sunDirUniform = glGetUniformLocation(program, "u_sunDirection");
    GLuint sunColorUniform = glGetUniformLocation(program, "u_sunColor");
    GLuint sunIntensityUniform = glGetUniformLocation(program, "u_sunIntensity");

    glm::vec4 dir = getLightDirection();
    glUniform4f(sunDirUniform, dir.x, dir.y, dir.z, dir.w);
    glUniform4f(sunColorUniform, sunColor.x, sunColor.y, sunColor.z, sunColor.w);
    glUniform1f(sunIntensityUniform, sunIntensity);
}

void DirectionalLight::sendCascadeClipEnds(GLuint uniformLocation)
{
    glUniform1fv(uniformLocation, 4, cascadeDistances);
}

void DirectionalLight::sendCascadeCount(GLuint uniformLocation)
{
    glUniform1i(uniformLocation, cascadeCount);
}*/

void DirectionalLight::sendLightMatrix(GLuint uniformLocation, int index)//N�o utilizado, ligh matrices s�o mandadas no UBO
{
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrices[index]));
}

void DirectionalLight::applyGlobalTransform()
{
    //Camera::applyGlobalTransform();
    //resetRotation();
    //rotateGlobalY(XRot);
    //rotateGlobalX(YRot);
    appliedTransform = parent->getGlobalTransform() * positionMatrix * scaleMatrix * rotationMatrix;
}

/*void DirectionalLight::sendShadowSettings(GLuint program)
{
    GLint biasUniform = glGetUniformLocation(program ,"shadowBias");
    GLint biasMult = glGetUniformLocation(program, "biasSplitMultiplier");
    GLint numSamplesUniform = glGetUniformLocation(program, "shadowSamples");

    glUniform1f(biasUniform, shadowBias);
    glUniform1f(biasMult, cascadeShadowBiasMultiplier);
    glUniform1i(numSamplesUniform, shadowSamples);

    GLint lightSpaceUniform = glGetUniformLocation(program, "cascadeMatrices");
    GLint directionalShadowMapUniform = glGetUniformLocation(program, "directionalShadowMap");
    GLint cascadeEndsUniform = glGetUniformLocation(program, "cascadePlaneDistances");
    GLint cascadeCountUniform = glGetUniformLocation(program, "cascadeCount");

    sendLightMatrices(lightSpaceUniform);
    sendShadowTextures(directionalShadowMapUniform);
    sendCascadeClipEnds(cascadeEndsUniform);
    sendCascadeCount(cascadeCountUniform);
}*/



