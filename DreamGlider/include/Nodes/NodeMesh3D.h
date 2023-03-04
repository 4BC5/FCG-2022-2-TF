#ifndef NODEMESH3D_H
#define NODEMESH3D_H
#include <glad/glad.h>
#include <Nodes/Node3D.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <tiny_obj_loader.h>
#include <Material.h>


class NodeMesh3D : public Node3D
{
    public:
        NodeMesh3D(std::string name, std::string modelPath, Material* material);
        //NodeMesh3D(std::string modelPath, std::string shaderPath);
        virtual ~NodeMesh3D();

        int vertexCount;
        std::vector<glm::vec4> vertices;
        std::vector<glm::vec4> normals;
        std::vector<glm::vec4> tangents;
        //std::vector<glm::vec4> bitangents;
        std::vector<glm::vec2> uvs;
        std::vector<GLuint> triangles;

        void loadMesh(std::string meshLocation);
        std::vector<glm::vec4>* getMeshPoints(){return &vertices;}
        std::vector<glm::vec4>* getMeshNormals(){return &normals;}
        std::vector<GLuint>* getMeshTriangles(){return &triangles;}
        std::vector<glm::vec2>* getMeshUVs(){return &uvs;}
        Material* getMaterial(){return material;}
        //GLuint getVertexCount(){return vertexCount;}
        GLuint getVAO(){return VAO;}
        void setVAO(GLuint newVAO){VAO = newVAO;}
        void printTriangles();
        protected:

    private:
        Material* material;
        GLuint VAO = 0;
};

#endif // NODEMESH3D_H
