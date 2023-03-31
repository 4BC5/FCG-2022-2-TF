#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include <Node3D.h>
#include <Material.h>

class PointLight : public Node3D
{
    public:
        PointLight(std::string name);
        virtual ~PointLight();

        void sendLightInfo(GLuint program, Material* mat, int index);
        void setAttenuationRadius(float attenuationRadius);
        void setColor(glm::vec4 color){this->color = color;}
        void setIntensity(float intensity){this->intensity = intensity;}

    protected:

    private:
        glm::vec4 color = glm::vec4(1.0f);//Cor da luz
        float intensity = 1.0f;//Intesidade da luz
        float attenuationRadius = 4.0f;//Raio de atenuação da luz (usado para calcular atenuação linear, que depois é multiplicado por si mesmo para ter decaimento quadratico)

};

#endif // POINTLIGHT_H
