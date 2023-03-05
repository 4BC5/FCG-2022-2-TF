//Implementação da Curva de Bézier -> Funções

#include <Nodes/Node.h>
#include <Nodes/Node3D.h>
#include <Nodes/Curves.h>
#include <matrices.h>
#include <glm/mat4x4.hpp>
#include <sstream>

Curves::Curves(std::string name) : Node3D(name)
{

}

Curves::~Curves()
{
    //dtor
}

glm::vec3 Curves::inter_linear(glm::vec3 p1,glm::vec3 p2,float t)
{
    return p1 + t*(p2-p1);
}

glm::vec3 Curves::inter_quad(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,float t)
{
    return inter_linear(p1,p2,t)+t*(inter_linear(p2,p3,t) - inter_linear(p1,p2,t));
}

glm::vec3 Curves::inter_cub(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,glm::vec3 p4,float t)
{
    return inter_quad(p1,p2,p3,t)+t*(inter_quad(p2,p3,p4,t)-inter_quad(p1,p2,p3,t));
}

