#include "AABB.h"

AABB::AABB()
{
    //ctor
}

AABB::~AABB()
{
    //dtor
}

void AABB::calculateFromMesh(Mesh3D* mesh)
{
    using namespace glm;
    PX = -1000000.0f;
    PY = -1000000.0f;
    PZ = -1000000.0f;
    NX = 1000000.0f;
    NY = 1000000.0f;
    NZ = 1000000.0f;

    int vertexCount = mesh->vertices.size();
    for (int i = 0; i < vertexCount; i++)
    {
        vec4 cV = mesh->vertices[i];
        PX = cV.x > PX ? cV.x : PX;
        PY = cV.y > PY ? cV.y : PY;
        PZ = cV.z > PZ ? cV.z : PZ;
        NX = cV.x < NX ? cV.x : NX;
        NY = cV.y < NY ? cV.y : NY;
        NZ = cV.z < NZ ? cV.z : NZ;
    }
}
