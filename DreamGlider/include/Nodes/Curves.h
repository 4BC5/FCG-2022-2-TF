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
    protected:

    private:
};

#endif // CURVES_H
