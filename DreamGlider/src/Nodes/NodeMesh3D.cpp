#include "NodeMesh3D.h"

NodeMesh3D::NodeMesh3D(std::string name, std::string modelPath, Material* material) : Node3D(name)
{
    type = 2;
    VAO = 0;

    this->material = material;
    loadMesh(modelPath);
}

/*NodeMesh3D::NodeMesh3D(std::string modelPath, std::string shaderPath)
{
    type = 2;
    this->shaderPath = shaderPath;
    loadMesh(modelPath);
}*/

NodeMesh3D::~NodeMesh3D()
{
    //dtor
}

int findIndex(tinyobj::index_t index, std::vector<tinyobj::index_t> vec)
{
    for (unsigned int i; i < vec.size(); i++)
    {
        if (index.normal_index == vec[i].normal_index && index.texcoord_index == vec[i].texcoord_index && index.vertex_index == vec[i].vertex_index)
        {
            return i;
        }
    }
    return -1;
}

void NodeMesh3D::loadMesh(std::string meshLocation)
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

    std::vector<glm::vec4> tempVertices;
    std::vector<glm::vec2> tempUVs;
    std::vector<glm::vec4> tempNormals;
    std::vector<tinyobj::index_t> usedIndices;

    std::cout << "OBJ vertices" << attrib.vertices.size() << "\n";

    for (unsigned int i = 0; i < attrib.vertices.size()/3; i++)
    {
        int base = i * 3;
        tempVertices.push_back(glm::vec4(attrib.vertices[base], attrib.vertices[base + 1], attrib.vertices[base + 2], 1.0f));
    }

    for (unsigned int i = 0; i < attrib.texcoords.size()/2; i++)
    {
        int base = i * 2;
        tempUVs.push_back(glm::vec2(attrib.texcoords[base], attrib.texcoords[base + 1]));
    }

    for (unsigned int i = 0; i < attrib.normals.size()/3; i++)
    {
        int base = i * 3;
        tempNormals.push_back(glm::vec4(attrib.normals[base],attrib.normals[base + 1], attrib.normals[base + 2], 0.0f));
    }

    for (unsigned int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        usedIndices.push_back(shapes[0].mesh.indices[i]);
        vertices.push_back(tempVertices[shapes[0].mesh.indices[i].vertex_index]);
        uvs.push_back(tempUVs[shapes[0].mesh.indices[i].texcoord_index]);
        normals.push_back(tempNormals[shapes[0].mesh.indices[i].normal_index]);
        triangles.push_back(i);
    }

}

void NodeMesh3D::printTriangles()
{
    //std::cout << "Pointer: " << getMeshTriangles() << "\n";
    std::cout << "Triangles: " << getMeshTriangles()->size() << "\n";
    for (unsigned int i = 0; i < getMeshTriangles()->size(); i++)
        std::cout << i << " : " << (*getMeshTriangles())[i] << "\n";
}
