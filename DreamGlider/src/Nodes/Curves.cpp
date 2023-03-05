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

glm::vec3 Curves::interpolateLin(glm::vec3 p1,glm::vec3 p2,float t)
{
    return p1 + t*(p2-p1);
}

glm::vec3 Curves::interpolateQuad(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,float t)
{
    return interpolateLin(p1,p2,t)+t*(interpolateLin(p2,p3,t) - interpolateLin(p1,p2,t));
}

glm::vec3 Curves::interpolateCub(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,glm::vec3 p4,float t)
{
    return interpolateQuad(p1,p2,p3,t)+t*(interpolateQuad(p2,p3,p4,t)-interpolateQuad(p1,p2,p3,t));
}

glm::vec3 Curves::interpolateTime(float t)
{
    return interpolateCub(this->A, this->B, this->C, this->D,t);
}
