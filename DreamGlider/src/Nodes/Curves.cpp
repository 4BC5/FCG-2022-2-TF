//Implementa��o da Curva de B�zier -> Fun��es

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
