#ifndef NODE3D_H
#define NODE3D_H

#include <Nodes/Node.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <matrices.h>
#include <AABB.h>

//Node responsável pelas transformações 3D dentro da cena
class Node3D : public Node
{
    public:
        Node3D(std::string name);
        virtual ~Node3D();

        glm::mat4 getTransform();
        glm::mat4 getGlobalTransform();

        void rotateAxis(glm::vec3 axis, float phi);


        //Rotação envolta de eixos locais do node
        void rotateLocalX(float phi);
        void rotateLocalY(float phi);
        void rotateLocalZ(float phi);

        //Rotações envolta de eixos globais
        void rotateGlobalX(float phi);
        void rotateGlobalY(float phi);
        void rotateGlobalZ(float phi);

        //Resetar a rotação
        void resetRotation();

        void translate(glm::vec3 translation);//Translação
        void globalTranslate(glm::vec3 translation);//Translação global
        //Translação em direção aos eixos base do node
        void localTranslateX(float distance);
        void localTranslateY(float distance);
        void localTranslateZ(float distance);

        //Escalamento do node
        void scale(glm::vec3 scaleAmount);

        //Definir posição local (dentro do pai) e global
        void setPosition(glm::vec3 position);
        void setGlobalPosition(glm::vec3 position);

        //Definir posição local em componentes separados
        void setPosX(float pos);
        void setPosY(float pos);
        void setPosZ(float pos);

        //Getters de posição e base
        glm::vec4 getPosition();
        glm::vec4 getGlobalPosition();
        glm::vec4 getBasisX();
        glm::vec4 getBasisY();
        glm::vec4 getBasisZ();
        glm::vec4 getGlobalBasisX();
        glm::vec4 getGlobalBasisY();
        glm::vec4 getGlobalBasisZ();

        //Função que aplica o transform global
        void applyGlobalTransform();

        //Função de definir o AABB
        void setAABB(AABB newAABB){aabb = newAABB;}
        AABB& getAABB(){return aabb;}
    protected:
        glm::mat4 positionMatrix = mop::Matrix_Identity();//Matriz de posição
        glm::mat4 rotationMatrix = mop::Matrix_Identity();//Matriz de rotação
        glm::mat4 scaleMatrix = mop::Matrix_Identity();//Matriz de escalagem
        glm::mat4 appliedTransform = mop::Matrix_Identity();//Matriz global
        AABB aabb = AABB(1.0f,1.0f,1.0f);//AABB do node
    private:
};

#endif // NODE3D_H
