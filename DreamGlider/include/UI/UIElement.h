//Classe para as funções de User Interface

#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <Renderer/Renderer.h>
#include <Nodes/Node.h>
#include <Renderer/Camera/Camera.h>
#include <Nodes/NodeMesh3D.h>
#include <Window/Window.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <fstream>
#include <matrices.h>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <Material.h>
#include <utils.h>
#include <stb_image.h>

//#include <dejavufont.h>


class UIElement
{
    public:
        UIElement();
        virtual ~UIElement();


    protected:

    private:
};

#endif // UIELEMENT_H
