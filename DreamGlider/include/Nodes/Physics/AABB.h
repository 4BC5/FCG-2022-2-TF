#ifndef AABB_H
#define AABB_H
#include <Mesh3D.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

class Camera;
class Window;

class AABB
{
    public:
        AABB(Mesh3D* mesh);
        AABB(float height, float width, float depth);
        AABB(glm::vec3 center, float height, float width, float depth);
        virtual ~AABB();

        getAABBTransformed(glm::mat4 trs);
        void calculateFromMesh(Mesh3D* mesh);
        void expandAABB(glm::vec3 expansion);
        bool AABBtoAABBtest(AABB& intersectWith);

        glm::vec3 getMaxPoint(){return transformedMaxPoint;}
        glm::vec3 getMinPoint(){return transformedMinPoint;}

        void updateTransform(glm::mat4 trs);

        void drawAABB(Camera* camera, Window* window);

    protected:
        glm::vec3 minPoint = glm::vec3(0.0f);
        glm::vec3 maxPoint = glm::vec3(0.0f);
        glm::vec3 transformedMaxPoint = glm::vec3(0.0f);
        glm::vec3 transformedMinPoint = glm::vec3(0.0f);
    private:
};

#endif // AABB_H
