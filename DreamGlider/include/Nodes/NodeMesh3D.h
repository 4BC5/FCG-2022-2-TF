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

//Classe derivada de Node3D que contém variáveis necessárias para renderização
class NodeMesh3D : public Node3D
{
    public:
        NodeMesh3D(std::string name, Mesh3D* mesh, Material* material);
        //NodeMesh3D(std::string modelPath, std::string shaderPath);
        virtual ~NodeMesh3D();

        Mesh3D* getMesh(){return mesh;}//Pegar a Mesh3D do node
        Material* getMaterial(){return material;}//Pegar o material do node
        bool getCastsShadows(){return castsShadows;}//Pegar se o node produz sombra
        void setEnvironmnetStrength(float environmnetStrength){this->environmnetStrength = environmnetStrength;}//Definir potência do environment mapping

        float getEnvironmnetStrength(){return environmnetStrength;}//Definir potência do environment mapping

        void setCastsShadows(bool castsShadows){this->castsShadows = castsShadows;}//Definir se o node produz sombra
        protected:

    private:
        Mesh3D* mesh;//A malha que vai ser renderizada
        Material* material;//O material a ser aplicado
        bool castsShadows = true;//Se o node produz sombra
        float environmnetStrength = 1.0f;//Potência do environment mapping
};

#endif // NODEMESH3D_H
