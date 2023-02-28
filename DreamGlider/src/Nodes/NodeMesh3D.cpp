#include "NodeMesh3D.h"

NodeMesh3D::NodeMesh3D(std::string modelPath)
{
    type = 2;
    this->shaderPath = "src/Shaders/main";
    loadMesh(modelPath);
}

NodeMesh3D::NodeMesh3D(std::string modelPath, std::string shaderPath)
{
    type = 2;
    this->shaderPath = shaderPath;
    loadMesh(modelPath);
}

NodeMesh3D::~NodeMesh3D()
{
    //dtor
}

typedef struct _triangleVertex
{
    GLuint vertexIndex;
    GLuint UVIndex;
    GLuint vertexNormal;

    bool compare(_triangleVertex cmp)
    {
        return cmp.vertexIndex == vertexIndex && cmp.UVIndex == UVIndex && cmp.vertexNormal == vertexNormal;
    }
}triangleVertex;

void NodeMesh3D::loadMesh(std::string meshLocation)
{
    std::ifstream file(meshLocation, std::ios::in);
    if (!file.is_open())
        return;

    std::vector<glm::vec4> tempVertices;
    std::vector<glm::vec4> tempNormals;
    std::vector<glm::vec2> tempUVs;
    std::vector<GLuint> triangleIdices;
    std::vector<triangleVertex> tempTriangleVertices;

    std::string currentLine;

    while (std::getline(file, currentLine))
    {
        std::stringstream lineStream(currentLine);
        std::string token;
        std::getline(lineStream, token, ' ');

        if (token[0] == '#')
            continue;

        if (token.compare("v") == 0)//Get vertices
        {
            glm::vec4 vert;
            vert.w = 1.0f;

            std::getline(lineStream, token, ' ');
            vert.x = std::stof(token);
            std::getline(lineStream, token, ' ');
            vert.y = std::stof(token);
            std::getline(lineStream, token, ' ');
            vert.z = std::stof(token);

            tempVertices.push_back(vert);
        }

        else if (token.compare("vt") == 0)//get
        {
            glm::vec2 uv;

            std::getline(lineStream, token, ' ');
            uv.x = std::stof(token);
            std::getline(lineStream, token, ' ');
            uv.y = std::stof(token);

            tempUVs.push_back(uv);
        }

        else if (token.compare("vn") == 0)
        {
            glm::vec4 norm;
            norm.w = 0.0f;

            std::getline(lineStream, token, ' ');
            norm.x = std::stof(token);
            std::getline(lineStream, token, ' ');
            norm.y = std::stof(token);
            std::getline(lineStream, token, ' ');
            norm.z = std::stof(token);

            tempNormals.push_back(norm);
        }
        else if (token.compare("f") == 0)
        {
            while (std::getline(lineStream, token, ' '))
            {
                if (token.find_first_not_of("0123456789- ") == std::string::npos)
                {
                    std::stringstream triangleStream(token);
                    std::string triangleInfo;
                    triangleVertex vertex;

                    std::getline(triangleStream, triangleInfo, '/');
                    vertex.vertexIndex = std::stoi(triangleInfo);
                    std::getline(triangleStream, triangleInfo, '/');
                    vertex.UVIndex = std::stoi(triangleInfo);
                    std::getline(triangleStream, triangleInfo, '/');
                    vertex.vertexNormal = std::stoi(triangleInfo);

                    bool foundVertex = false;
                    for (unsigned int i = 0; i < tempTriangleVertices.size(); i++)
                    {
                        if (tempTriangleVertices[i].compare(vertex))
                        {
                            foundVertex = true;
                            triangleIdices.push_back(i);
                            break;
                        }
                    }
                    if (!foundVertex)
                    {
                        tempTriangleVertices.push_back(vertex);
                        triangleIdices.push_back(tempTriangleVertices.size() - 1);
                    }
                }
            }
        }
        else
        {
            continue;
        }
    }

    vertexCount = tempTriangleVertices.size();
    vertices.resize(vertexCount);
    uvs.resize(vertexCount);
    normals.resize(vertexCount);

    for (int i = 0; i < vertexCount; i++)
    {
        triangleVertex vert = tempTriangleVertices[i];
        vertices[i] = tempVertices[vert.vertexIndex - 1];
        uvs[i] = tempUVs[vert.UVIndex - 1];
        normals[i] = tempNormals[vert.vertexNormal - 1];
    }
}
