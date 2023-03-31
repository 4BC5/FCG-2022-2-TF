#ifndef MESH3D_H
#define MESH3D_H
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <vector>
#include <tiny_obj_loader.h>
#include <iostream>

//Classe que carrega as malhas poligonais
class Mesh3D
{
    public:
        Mesh3D(const std::string& meshPath);
        virtual ~Mesh3D();

        std::vector<GLuint> triangles;//Triângulos (indices de vértices)
        std::vector<glm::vec4> vertices;//Vértices
        std::vector<glm::vec4> normals;//Normais
        std::vector<glm::vec4> tangents;//Tangentes (para normal mapping)
        std::vector<glm::vec2> uvs;//UVs para texturas

        GLuint getVAO(){return meshVAO;}

        void setVAO(GLuint VAO){this->meshVAO = VAO;}


    protected:


    private:
        void loadMesh(std::string meshLocation);

        GLuint meshVAO = 0;//VAO da Mesh3D

};

#endif // MESH3D_H
