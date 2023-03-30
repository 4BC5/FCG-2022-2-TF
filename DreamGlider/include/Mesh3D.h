#ifndef MESH3D_H
#define MESH3D_H
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <vector>
#include <tiny_obj_loader.h>
#include <iostream>

class Mesh3D
{
    public:
        Mesh3D(const std::string& meshPath);
        virtual ~Mesh3D();

        std::vector<GLuint> triangles;
        std::vector<glm::vec4> vertices;
        std::vector<glm::vec4> normals;
        std::vector<glm::vec4> tangents;
        std::vector<glm::vec2> uvs;

        GLuint getVAO(){return meshVAO;}

        void setVAO(GLuint VAO){this->meshVAO = VAO;}


    protected:


    private:
        void loadMesh(std::string meshLocation);

        GLuint meshVAO = 0;

};

#endif // MESH3D_H
