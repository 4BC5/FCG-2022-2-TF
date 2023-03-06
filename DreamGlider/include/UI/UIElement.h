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



class UIElement
{
    public:
        UIElement(Window* window);
        virtual ~UIElement();

        float screenPosX, screenPosY; //Posição do elemento na tela

        //Objeto teste para renderização: Triângulo
        //GLuint objeto_teste;
  /*      GLfloat NDC_coefficients[] = {
              screenPosX, -screenPosY,0.0f,1.0f,
              screenPosX+1.0f, -screenPosY,0.0f,1.0f,
              screenPosX,  screenPosY,0.0f,1.0f,
              screenPosX+1.0,  screenPosY,0.0f,1.0f,
            };
*/

/*

{int i, indi=5;

    // "NDC_coefficients" define a GEOMETRIA
    float posx = 0.03f, posy=0.2f, pos_des;
    pos_des = -posx+desloca;
    posx+=desloca;


    GLfloat NDC_coefficients[] = {
    // X      Y       Z    W
      posx, -posy,0.0f,1.0f,
      pos_des, -posy,0.0f,1.0f,
      posx,  posy,0.0f,1.0f,
      pos_des,  posy,0.0f,1.0f,
    };

    GLuint VBO_NDC_coefficients_id;
    glGenBuffers(1, &VBO_NDC_coefficients_id);
    GLuint vertex_array_object_id;
    glGenVertexArrays(1, &vertex_array_object_id);
    glBindVertexArray(vertex_array_object_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_NDC_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(NDC_coefficients), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(NDC_coefficients), NDC_coefficients);

    GLuint location = 0;
    GLint  number_of_dimensions = 4;
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Cor: Vermelho, Verde, Azul, Alpha (valor de transparência).
    float um = 1.0f, zero = 0.0f;
    GLfloat color_coefficients[]= {
    //  R     G     B     A (=1)
            zero,zero,um,um,
            zero,zero,um,um,
            zero,zero,um,um,
            zero,zero,um,um,
    };

    GLuint VBO_color_coefficients_id;
    glGenBuffers(1, &VBO_color_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_coefficients), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color_coefficients), color_coefficients);
    location = 1;
    number_of_dimensions = 4;
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // "indices" define a TOPOLOGIA
    GLubyte indices[indi]; // GLubyte: valores entre 0 e 255 (8 bits sem sinal).
    for (i=0;i<indi;i++)
    {
        indices[i] = i;
    }


    GLuint indices_id;
    glGenBuffers(1, &indices_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);
    glBindVertexArray(0);

    return vertex_array_object_id;
}





        cube->vertices = {glm::vec4(-0.5f, -0.5f, -1.0f, 1.0f),
                      glm::vec4(0.5f, -0.5f, -1.0f, 1.0f),
                      glm::vec4(0.5f, 0.5f, -1.0f, 1.0f),};
    cube->triangles = {2,0,1};

    cube->normals = {glm::vec4(0.0f, 0.0f, -1.0f, 0.0f),
                     glm::vec4(0.0f, 0.0f, -1.0f, 0.0f),
                     glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)};

                      GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint g_GpuProgramID = renderer.loadGPUProgram(cube->getShaderPath());

    GLuint verticesVBOID;
    glGenBuffers(1, &verticesVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBOID);//Bind vertices VBO

    glBufferData(GL_ARRAY_BUFFER, cube->vertices.size() * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);//Allocate memory for the vertices VBO on the GPU
    glBufferSubData(GL_ARRAY_BUFFER, 0, cube->vertices.size() * sizeof(glm::vec4), &cube->vertices[0]);//Copy vertices VBO to VRAM memory

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);//Define vertex attribute data
    glEnableVertexAttribArray(0);//Enable vertex attribute data
    glBindBuffer(GL_ARRAY_BUFFER, 0);//Disable VBO

    GLuint indices;
    glGenBuffers(1, &indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube->triangles.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, cube->triangles.size() * sizeof(GLuint), &cube->triangles[0]);

    glUseProgram(g_GpuProgramID);
*/

//retângulo p renderizar texturas
//e p texto
//matriz de transformação

//posição na tela
//derivada: texto

//Desenha triângulo na tela
//ajustar p aspecto da tela (window->getaspect)

//alfabeto como textura
//escalonar UV p ficar do tamanho de uma letra e faz offset

    protected:

    private:
};

#endif // UIELEMENT_H
