//Classe para as funções de Texto na tela
//Baseada em TextRenderer.cpp disponibilizado pelos labs

#ifndef TEXT_H
#define TEXT_H

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


class Text //: public Renderer
{
    public:
        Text(Renderer* renderer);//(Window* window, Camera* cam, Node* root, Camera* directional);
        virtual ~Text();

        //Arquivo text_vertex.glsl
        const GLchar* const textvertexshader_source = ""
        "#version 330\n"
        "layout (location = 0) in vec4 position;\n"
        "out vec2 texCoords;\n"
        "void main()\n"
        "{\n"
            "gl_Position = vec4(position.xy, 0, 1);\n"
            "texCoords = position.zw;\n"
        "}\n"
        "\0";
        //Arquivo text_fragment.glsl
        const GLchar* const textfragmentshader_source = ""
        "#version 330\n"
        "uniform sampler2D tex;\n"
        "in vec2 texCoords;\n"
        "out vec4 fragColor;\n"
        "void main()\n"
        "{\n"
            "fragColor = vec4(0, 0, 0, texture(tex, texCoords).r);\n"
        "}\n"
        "\0";

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

#endif // TEXT_H
