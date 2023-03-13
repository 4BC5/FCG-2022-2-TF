#ifndef AABB_H
#define AABB_H
#include <Mesh3D.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

class AABB
{
    public:
        AABB();
        virtual ~AABB();
        getAABBTransformed(glm::mat4 trs);
        void calculateFromMesh(Mesh3D* mesh);

    protected:

    private:
        float PX = 0.0f;
        float PY = 0.0f;
        float PZ = 0.0f;
        float NX = 0.0f;
        float NY = 0.0f;
        float NZ = 0.0f;
};

#endif // AABB_H
