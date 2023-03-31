#ifndef AABB_H
#define AABB_H
#include <Mesh3D.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

class Camera;
class Window;
//Classe de AABB, possibilita encontrar proximidade entre nodes de modo rápido
//Ajuda a acelerar física e permite que cada NodeMesh3D tenha até 16 point lights diferentes iluminando
class AABB
{
    public:
        AABB(Mesh3D* mesh);//Criar AABB a partir de uma Mesh3D
        AABB(float height, float width, float depth);//Criar AABB a partir de tamanhos
        AABB(glm::vec3 center, float height, float width, float depth);//Criar AABB a partir de tamanhos com offset
        virtual ~AABB();

        getAABBTransformed(glm::mat4 trs);//Retorna o AABB transformado pela matriz "trs"
        void calculateFromMesh(Mesh3D* mesh);//Calcula o AABB a partir de uma Mesh3D
        void expandAABB(glm::vec3 expansion);//Expante o AABB pelo vetor "expansion"
        bool AABBtoAABBtest(AABB& intersectWith);//Testa se collide com o AABB "intersectWith"

        glm::vec3 getMaxPoint(){return transformedMaxPoint;}//Pega o ponto máximo do AABB
        glm::vec3 getMinPoint(){return transformedMinPoint;}//Pega o pnto mínimo do AABB

        void updateTransform(glm::mat4 trs);//Faz update do AABB usando a matriz "trs"

        void drawAABB(Camera* camera, Window* window);//Desenha o AABB na tela para debug
        void combineAABB(AABB comb, glm::vec3 center = glm::vec3(0.0f));//Combina AABBs

    protected:
        glm::vec3 minPoint = glm::vec3(0.0f);//Ponto mínimo do AABB
        glm::vec3 maxPoint = glm::vec3(0.0f);//Ponto máximo do AABB
        glm::vec3 transformedMaxPoint = glm::vec3(0.0f);//Ponto máximo do AABB em coordenadas globais
        glm::vec3 transformedMinPoint = glm::vec3(0.0f);//Ponto mínimo do AABB em coordenadas globais

        std::vector<glm::vec3> getCorners(AABB& aabb);//Pega os cantos do AABB "aabb"
    private:
};

#endif // AABB_H
