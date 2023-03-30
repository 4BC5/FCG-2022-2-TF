#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include <Node3D.h>
#include <Material.h>
//Classe de luz pontual
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
        glm::vec4 color = glm::vec4(1.0f);
        float intensity = 1.0f;
        float attenuationRadius = 4.0f;

};

#endif // POINTLIGHT_H
