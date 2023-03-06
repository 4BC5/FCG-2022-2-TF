#ifndef UITEXT_H
#define UITEXT_H

#include <UI/UIElement.h>
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

class UIText : public UIElement
{
    public:
        UIText();
        virtual ~UIText();
        //Arquivo text_vertex.glsl
        //Arquivo text_fragment.glsl

        GLuint textVAO;
        GLuint textVBO;
        GLuint textprogram_id;
        GLuint texttexture_id;

        float textscale = 1.5f;

        void TextRendering_LoadShader(const GLchar* const shader_string, GLuint shader_id);
        void TextRendering_Init(Renderer* renderer);
        void TextRendering_PrintString(GLFWwindow* window, const std::string &str, float x, float y, float scale);
        float TextRendering_LineHeight(GLFWwindow* window);
        float TextRendering_CharWidth(GLFWwindow* window);
        void TextRendering_ShowFramesPerSecond(GLFWwindow* window);

    protected:

    private:
};

#endif // UITEXT_H
