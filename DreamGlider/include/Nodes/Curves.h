//Implementação da Curva de Bézier -> Classe

#ifndef CURVES_H
#define CURVES_H

#include <Nodes/Node.h>
#include <Nodes/Node3D.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <matrices.h>


class Curves : public Node3D
{
    public:
        Curves(std::string name);
        virtual ~Curves();

        //Funções Interpolação
        glm::vec3 inter_linear(glm::vec3 p1,glm::vec3 p2,float t);
        glm::vec3 inter_quad(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,float t);
        glm::vec3 inter_cub(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,glm::vec3 p4,float t);

        //Pontos fixos
        glm::vec3 A = glm::vec3(0.0f,0.0f,0.0f);
        glm::vec3 B = glm::vec3(2.0f,1.0f,2.0f);
        glm::vec3 C = glm::vec3(-2.0f,2.0f,-2.0f);
        glm::vec3 D = glm::vec3(0.0f,3.0f,0.0f);

        //Constrói curva


    protected:

    private:
};

#endif // CURVES_H
