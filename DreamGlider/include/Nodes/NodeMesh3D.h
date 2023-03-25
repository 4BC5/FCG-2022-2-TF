#ifndef NODEMESH3D_H
#define NODEMESH3D_H
#include <glad/glad.h>
#include <Nodes/Node3D.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <tiny_obj_loader.h>
#include <Material.h>
#include <Mesh3D.h>


class NodeMesh3D : public Node3D
{
    public:
        NodeMesh3D(std::string name, Mesh3D* mesh, Material* material);
        //NodeMesh3D(std::string modelPath, std::string shaderPath);
        virtual ~NodeMesh3D();

        int vertexCount;

        Mesh3D* getMesh(){return mesh;}
        Material* getMaterial(){return material;}
        bool getCastsShadows(){return castsShadows;}
        void setEnvironmnetStrength(float environmnetStrength){this->environmnetStrength = environmnetStrength;}

        float getEnvironmnetStrength(){return environmnetStrength;}

        void setCastsShadows(bool castsShadows){this->castsShadows = castsShadows;}
        protected:

    private:
        Mesh3D* mesh;
        Material* material;
        bool castsShadows = true;
        float environmnetStrength = 1.0f;
};

#endif // NODEMESH3D_H
