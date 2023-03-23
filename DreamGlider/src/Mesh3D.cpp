#include "Mesh3D.h"

Mesh3D::Mesh3D(const std::string& meshPath)
{
    loadMesh(meshPath);
}

Mesh3D::~Mesh3D()
{
    //dtor
}


bool compareIndices(tinyobj::index_t index1, tinyobj::index_t index2)
{
    return (index1.normal_index == index2.normal_index && index1.texcoord_index == index2.texcoord_index && index1.vertex_index == index2.vertex_index);
}

void Mesh3D::loadMesh(std::string meshLocation)
{
    tinyobj::attrib_t attrib;

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool loaded = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, meshLocation.c_str(),NULL,true);

    if (!warn.empty())
    {
        std::cerr << "Mesh load warning: " << warn << "\n";
    }

    if (!err.empty())
    {
        std::cerr << "Mesh load error: " << err << "\n";
    }

    if (!loaded)
    {
        std::cout << "Couldn't load mesh.\n";
    }

    //std::cout << "OBJ vertices:" << attrib.vertices.size() << "\n";

    std::vector<glm::vec4> tempVertices;
    std::vector<glm::vec2> tempUVs;
    std::vector<glm::vec4> tempNormals;
    std::vector<tinyobj::index_t> usedIndices;

    tempVertices.resize(attrib.vertices.size() / 3);
    tempUVs.resize(attrib.texcoords.size()/2);
    tempNormals.resize(attrib.normals.size()/3);

    for (unsigned int i = 0; i < attrib.vertices.size()/3; i++)
    {
        int base = i * 3;
        tempVertices[i] = glm::vec4(attrib.vertices[base], attrib.vertices[base + 1], attrib.vertices[base + 2], 1.0f);
    }

    for (unsigned int i = 0; i < attrib.texcoords.size()/2; i++)
    {
        int base = i * 2;
        tempUVs[i] = glm::vec2(attrib.texcoords[base], attrib.texcoords[base + 1]);
    }

    for (unsigned int i = 0; i < attrib.normals.size()/3; i++)
    {
        int base = i * 3;
        tempNormals[i] = glm::vec4(attrib.normals[base],attrib.normals[base + 1], attrib.normals[base + 2], 0.0f);
    }


    for (unsigned int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        tinyobj::index_t currentIndex = shapes[0].mesh.indices[i];

        bool found = false;
        for (unsigned int j = 0; j < usedIndices.size(); j++)
        {
            found = compareIndices(currentIndex, usedIndices[j]);
            if (found)
            {
                triangles.push_back(j);
                break;
            }
        }
        if (!found)
        {
            vertices.push_back(tempVertices[currentIndex.vertex_index]);
            normals.push_back(tempNormals[currentIndex.normal_index]);
            uvs.push_back(tempUVs[currentIndex.texcoord_index]);
            usedIndices.push_back(currentIndex);
            triangles.push_back(vertices.size() - 1);
        }
    }

    tangents.resize(triangles.size());
    std::fill(tangents.begin(), tangents.end(), glm::vec4(0.0f,0.0f,0.0f,0.0f));

    //bitangents.resize(vertices.size());
    //std::fill(bitangents.begin(), bitangents.end(), glm::vec4(0.0f));

    for (unsigned int i = 0; i < triangles.size()/3; i++)
    {
        unsigned int base = i * 3;
        unsigned int vert1Index = triangles[base];
        unsigned int vert2Index = triangles[base + 1];
        unsigned int vert3Index = triangles[base + 2];

        glm::vec4 pos1 = vertices[vert1Index];
        glm::vec4 pos2 = vertices[vert2Index];
        glm::vec4 pos3 = vertices[vert3Index];
        glm::vec2 uv1 = uvs[vert1Index];
        glm::vec2 uv2 = uvs[vert2Index];
        glm::vec2 uv3 = uvs[vert3Index];


        glm::vec4 edge1 = pos2 - pos1;
        glm::vec4 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec4 tangent;
        tangent.w = 0.0f;

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


        tangents[vert1Index] += tangent;
        tangents[vert2Index] += tangent;
        tangents[vert3Index] += tangent;
    }

}
