#include "Renderer.h"

#define L_VERTICES 0
#define L_UVS 1
#define L_NORMALS 2
#define L_TANGENTS 3
#define L_BITANGENTS 4
#define D_VERTICES 4
#define D_UVS 2
#define D_NORMALS 4
#define D_TANGENTS 4
#define D_BITANGENTS 4


#define SHADOW_WIDTH 2048
#define SHADOW_HEIGHT 2048


void Renderer::setUpShadowMap()
{
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f,1.0f,1.0f,1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    depthProgram = loadGPUProgram(SHADER_DEPTH);
    depthDiscardProgram = loadGPUProgram(SHADER_DEPTH_ALPHA_DISCARD);
    glCheckError();

}

Renderer::Renderer(Window* window, Camera* cam, Node* root, Camera* directional)
{

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glCheckError();

    sceneRoot = root;
    this->window = window;
    camera = cam;

    glEnable(GL_DEPTH_TEST);//Enable depth buffer
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glCheckError();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    this->directional = directional;
    if (directional != nullptr)
    {
        setUpShadowMap();
    }
}

Renderer::~Renderer()
{
    //dtor
}

void Renderer::render()
{
    /////////////Render shadow map
    glCheckError();
    if (directional != nullptr)
    {
        glViewport(0,0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        renderShadowMap(sceneRoot);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glCheckError();
    }
    //Render main scene
    glViewport(0 , 0, window->getWidth(), window->getHeigth());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Clear depth buffer
    renderObject(sceneRoot);
    glfwSwapBuffers(window->getWindow());
    glCheckError();
}

void Renderer::renderShadowMap(Node* object)
{
    switch (object->type)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        NodeMesh3D* meshNode = static_cast<NodeMesh3D*>(object);
        GLuint VAOId = buildMesh(meshNode);//Build VAO

        glBindVertexArray(VAOId);//Bind VAO
        GLuint gpuProgram;
        bool depthDiscard = meshNode->getMaterial()->shaderType == SHADER_BLINN_PHONG_ALPHA_DISCARD;
        if (depthDiscard)
        {
            gpuProgram = depthDiscardProgram;
            if (meshNode->getMaterial()->faceCulling)
            {
                glEnable(GL_CULL_FACE);
                glCullFace(meshNode->getMaterial()->faceCullingMode);
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }
        }
        else
        {
            gpuProgram = depthProgram;
        }


        glUseProgram(gpuProgram);//Set GPU program handle to use
        GLint modelUniform           = glGetUniformLocation(gpuProgram, "model"); // Variável da matriz "model"
        GLint viewUniform            = glGetUniformLocation(gpuProgram, "lightSpaceMatrix"); // Variável da matriz "view" em shader_vertex.glsl


        if (depthDiscard)
        {
            loadMaterial(meshNode->getMaterial());
            GLint UVtilingUniform = glGetUniformLocation(gpuProgram, "UVtiling");
            GLint alphaTexUniform = glGetUniformLocation(gpuProgram, "alphaTex");

            glCheckError();

            GLuint textureID = meshNode->getMaterial()->albedoTexIndex;
            glCheckError();
            glActiveTexture(GL_TEXTURE1);
            glCheckError();
            glBindTexture(GL_TEXTURE_2D, textureID);
            glCheckError();
            glUniform1i(alphaTexUniform, 1);
            glCheckError();
            glUniform2f(UVtilingUniform, meshNode->getMaterial()->UVtiling.x, meshNode->getMaterial()->UVtiling.y);
            glCheckError();
        }

        glm::mat4 globalTransform = meshNode->getGlobalTransform();
        glm::mat4 lightSpaceMatrix = directional->getLightSpaceMatrix(20.0f);

        //mop::PrintMatrix(globalTransform);

        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(globalTransform));
        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

        glDrawElements(GL_TRIANGLES, meshNode->triangles.size(), GL_UNSIGNED_INT, 0);

        glUseProgram(0);
        glBindVertexArray(0);

        break;
    }

    int childCount = object->children.size();
    if (childCount > 0)
    {
        for (int i = 0; i < childCount; i++)
        {
            renderShadowMap(object->children[i]);
        }
    }
}

void Renderer::renderObject(Node* object)
{
    //std::cout << "Object: " << object->name << "\n";
    //std::cout << "Object type: " << object->type << "\n";
    switch (object->type)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:

        NodeMesh3D* meshNode = static_cast<NodeMesh3D*>(object);
        glCheckError();
        GLuint g_GpuProgramID = loadMaterial(meshNode->getMaterial());//Load GPU program
        glCheckError();

        GLuint VAOId = buildMesh(meshNode);//Build VAO


        glBindVertexArray(VAOId);//Bind VAO
        glCheckError();
        glUseProgram(g_GpuProgramID);//Set GPU program handle to use
        GLint modelUniform         = glGetUniformLocation(g_GpuProgramID, "model"); // Variável da matriz "model"
        GLint viewUniform          = glGetUniformLocation(g_GpuProgramID, "view"); // Variável da matriz "view" em shader_vertex.glsl
        GLint projectionUniform    = glGetUniformLocation(g_GpuProgramID, "projection");
        GLint lightSpaceUniform    = glGetUniformLocation(g_GpuProgramID, "lightSpaceMatrix");
        GLint shadowMapUniform     = glGetUniformLocation(g_GpuProgramID, "shadowMap");

        GLint normalStrengthUniform = glGetUniformLocation(g_GpuProgramID, "normalStrength");
        GLint uvTilingUniform = glGetUniformLocation(g_GpuProgramID, "UVTiling");

        glUniform1f(normalStrengthUniform, meshNode->getMaterial()->normalStrength);
        glUniform2f(uvTilingUniform, meshNode->getMaterial()->UVtiling.x, meshNode->getMaterial()->UVtiling.y);

        ////////////Textures
        GLint albedoUniform = glGetUniformLocation(g_GpuProgramID, "albedoTexture");
        GLint normalUniform = glGetUniformLocation(g_GpuProgramID, "normalTexture");
        GLint roughnessUniform = glGetUniformLocation(g_GpuProgramID, "roughnessTexture");

        glm::mat4 projection = camera->getProjectionMatrix(window->getAspect());
        glm::mat4 view = camera->getCameraMatrix();
        glm::mat4 globalTransform = meshNode->getGlobalTransform();

        //mop::PrintMatrix(globalTransform);
        glCheckError();
        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(globalTransform));
        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));




        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,meshNode->getMaterial()->albedoTexIndex);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, meshNode->getMaterial()->normalTexIndex);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, meshNode->getMaterial()->normalTexIndex);

        glUniform1i(albedoUniform, 1);
        glUniform1i(normalUniform, 2);
        glUniform1i(roughnessUniform, 3);

        if (directional != nullptr)
        {
            glm::mat4 lightSpaceMatrix = directional->getLightSpaceMatrix(20.0f);
            glUniformMatrix4fv(lightSpaceUniform, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            glUniform1i(shadowMapUniform, 0);
            //glBindTexture(GL_TEXTURE_2D, 0);
        }

        glCheckError();

        if (meshNode->getMaterial()->faceCulling)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(meshNode->getMaterial()->faceCullingMode);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }

        glDrawElements(GL_TRIANGLES, meshNode->triangles.size(), GL_UNSIGNED_INT, 0);


        if (DEBUG)
        {
            glMatrixMode(GL_PROJECTION);
            glm::mat4 projMatrix = camera->getProjectionMatrix(window->getAspect());
            glLoadMatrixf((const GLfloat*)&projMatrix[0]);
            glMatrixMode(GL_MODELVIEW);
            glm::mat4 MV = camera->getCameraMatrix() * globalTransform;
            glLoadMatrixf((const GLfloat*)&MV[0]);
            glUseProgram(0);

            glColor3f(0,0,1);
            glBegin(GL_LINES);
            for (unsigned int i = 0; i < meshNode->triangles.size(); i++)
            {
                unsigned int index = meshNode->triangles[i];
                glm::vec4 p = meshNode->vertices[index];
                glVertex3fv(&p.x);
                glm::vec4 o = glm::normalize(meshNode->normals[index]);
                p+=o*0.1f;
                glVertex3fv(&p.x);
            }
            glColor3f(0,1,0);
            for (unsigned int i = 0; i < meshNode->triangles.size(); i++)
            {
                unsigned int index = meshNode->triangles[i];
                glm::vec4 p = meshNode->vertices[index];
                glVertex3fv(&p.x);
                glm::vec4 o = glm::normalize(meshNode->tangents[index]);
                p+=o*0.1f;
                glVertex3fv(&p.x);
            }

            glEnd();

        }


        glCheckError();

        glBindVertexArray(0);
        break;
    }

    int childCount = object->children.size();
    if (childCount > 0)
    {
        for (int i = 0; i < childCount; i++)
        {
            renderObject(object->children[i]);
        }
    }
}

GLuint Renderer::buildMesh(NodeMesh3D* meshNode)
{
    if (meshNode->getVAO() != 0)
    {
        return meshNode->getVAO();
    }

    GLuint VAOId;
    glGenVertexArrays(1, &VAOId);//Generate VAO id
    glBindVertexArray(VAOId);//Bind VAO

    GLuint verticesVBOID;
    glGenBuffers(1, &verticesVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBOID);//Bind vertices VBO

    glBufferData(GL_ARRAY_BUFFER, meshNode->vertices.size() * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);//Allocate memory for the vertices VBO on the GPU
    glBufferSubData(GL_ARRAY_BUFFER, 0, meshNode->vertices.size() * sizeof(glm::vec4), &meshNode->vertices[0]);//Copy vertices VBO to VRAM

    glVertexAttribPointer(L_VERTICES, D_VERTICES, GL_FLOAT, GL_FALSE, 0, 0);//Define vertex attribute data
    glEnableVertexAttribArray(L_VERTICES);//Enable vertex attribute data
    glBindBuffer(GL_ARRAY_BUFFER, 0);//Disable VBO


    //////////////////////////////////////// UVs


    GLuint UVsVBOId;
    glGenBuffers(1, &UVsVBOId);
    glBindBuffer(GL_ARRAY_BUFFER, UVsVBOId);
    glBufferData(GL_ARRAY_BUFFER, meshNode->uvs.size() * sizeof(glm::vec2), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, meshNode->uvs.size() * sizeof(glm::vec2), &meshNode->uvs[0]);
    glVertexAttribPointer(L_UVS, D_UVS, GL_FLOAT, GL_FALSE, 0, 0);//Position 1, 2 members
    glEnableVertexAttribArray(L_UVS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ///////////////////////////////////// Normals

    GLuint normalsVBOId;
    glGenBuffers(1, &normalsVBOId);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBOId);
    glBufferData(GL_ARRAY_BUFFER, meshNode->normals.size() * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, meshNode->normals.size() * sizeof(glm::vec4), &meshNode->normals[0]);
    glVertexAttribPointer(L_NORMALS, D_NORMALS, GL_FLOAT, GL_TRUE, 0, 0);
    glEnableVertexAttribArray(L_NORMALS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ///////////////////////////////////// Tangents

    GLuint tangentsVBOid;
    glGenBuffers(1, &tangentsVBOid);
    glBindBuffer(GL_ARRAY_BUFFER, tangentsVBOid);
    glBufferData(GL_ARRAY_BUFFER, meshNode->tangents.size() * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, meshNode->tangents.size() * sizeof(glm::vec3), &meshNode->tangents[0]);
    glVertexAttribPointer(L_TANGENTS, D_TANGENTS, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(L_TANGENTS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ///////////////////////////////////// Bitangents

    /*GLuint binormalsVBOid;
    glGenBuffers(1, &binormalsVBOid);
    glBindBuffer(GL_ARRAY_BUFFER, binormalsVBOid);
    glBufferData(GL_ARRAY_BUFFER, meshNode->bitangents.size() * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, meshNode->bitangents.size() * sizeof(glm::vec3), &meshNode->bitangents[0]);
    glVertexAttribPointer(L_BITANGENTS, D_BITANGENTS, GL_FLOAT, GL_TRUE, 0, 0);
    glEnableVertexAttribArray(L_BITANGENTS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);*/

    ///////////////////////////////////////Triangles

    GLuint indices;
    glGenBuffers(1, &indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshNode->triangles.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, meshNode->triangles.size() * sizeof(GLuint), &meshNode->triangles[0]);
    std::cout << "TRS: " << meshNode->triangles.size() << "\n";

    glBindVertexArray(0);
    meshNode->setVAO(VAOId);
    std::cout << "VAO: " << VAOId << "\n";
    return VAOId;
}

bool loadShader(std::string shaderPath, GLuint shaderID)
{
    std::ifstream shaderFile(shaderPath, std::ios::in);
    if (!shaderFile.is_open())
    {
        std::cout << "Could not open shader file:" << shaderPath << "\n";
        return false;
    }

    std::stringstream shaderText;
    shaderText << shaderFile.rdbuf();
    std::string shaderString = shaderText.str();
    const GLchar* shaderStringPointer = shaderString.c_str();
    const GLint   shaderStringLength = static_cast<GLint>( shaderString.length());

    glShaderSource(shaderID, 1, &shaderStringPointer, &shaderStringLength);
    glCompileShader(shaderID);

    GLint compiled_ok;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled_ok);

    GLint log_length = 0;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &log_length);
    GLchar* log = new GLchar[log_length];
    glGetShaderInfoLog(shaderID, log_length, &log_length, log);

    if ( log_length != 0 )
    {
        std::string  output;

        if ( !compiled_ok )
        {
            output += "ERROR: OpenGL compilation of \"";
            output += shaderPath;
            output += "\" failed.\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }
        else
        {
            output += "WARNING: OpenGL compilation of \"";
            output += shaderPath;
            output += "\".\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }

        std::cout << output;
    }
    delete [] log;

    return true;
}

GLuint loadVertexShader(std::string shaderPath)
{
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);

    loadShader(shaderPath + "_vertex.glsl", vertexShaderId);

    return vertexShaderId;
}

GLuint loadFragmentShader(std::string shaderPath)
{
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    loadShader(shaderPath + "_fragment.glsl", fragmentShaderId);

    return fragmentShaderId;
}

GLuint CreateGpuProgram(GLuint vertexShaderId, GLuint fragmentShaderId)
{
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);

    GLint linked_ok = GL_FALSE;
    glGetProgramiv(programId, GL_LINK_STATUS, &linked_ok);

    if ( linked_ok == GL_FALSE )
    {
        GLint log_length = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &log_length);

        // Alocamos memória para guardar o log de compilação.
        // A chamada "new" em C++ é equivalente ao "malloc()" do C.
        GLchar* log = new GLchar[log_length];

        glGetProgramInfoLog(programId, log_length, &log_length, log);

        std::string output;

        output += "ERROR: OpenGL linking of program failed.\n";
        output += "== Start of link log\n";
        output += log;
        output += "\n== End of link log\n";

        // A chamada "delete" em C++ é equivalente ao "free()" do C
        delete [] log;

        std::cout << output;
    }
    return programId;
}



GLuint Renderer::loadGPUProgram(int shaderType)
{
    for (unsigned int i = 0; i < loadedshaders.size(); i++)
    {
        if (loadedshaders[i].shaderType == shaderType)
        {
            return loadedshaders[i].programID;
        }
    }

    std::string path = shaderPaths[shaderType];

    GLuint vertexShaderId = loadVertexShader(path);

    GLuint fragmentShaderId = loadFragmentShader(path);


    GLuint GPUProgramId;

    GPUProgramId = CreateGpuProgram(vertexShaderId, fragmentShaderId);
    Shader newShader;
    newShader.shaderType = shaderType;
    newShader.programID = GPUProgramId;
    loadedshaders.push_back(newShader);
    return GPUProgramId;
}

GLuint Renderer::loadTexture(std::string path)
{
    for (unsigned int i = 0; i < loadedTextures.size(); i++)
    {
        if (path.compare(loadedTextures[i].texturePath) == 0)
        {
            return loadedTextures[i].textureId;
        }
    }
    std::cout << "Loading texture: " << path << "\n";

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (!data)
    {
        std::cout << "Could not load texture: " << path << "\n";
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint format;

    switch (nrChannels)
    {
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    Texture tex;
    tex.textureId = texture;
    tex.texturePath = path;


    loadedTextures.push_back(tex);
    return texture;
}

GLuint Renderer::loadMaterial(Material* material)
{
    if (material->albedoTexIndex == 0)
    {
        material->albedoTexIndex = loadTexture(material->albedoTexturePath);
    }
    if (material->normalTexIndex == 0)
    {
        material->normalTexIndex = loadTexture(material->normalMapPath);
    }
    if (material->roughnessTextureIndex == 0)
    {
        material->roughnessTextureIndex = loadTexture(material->roughnessMapPath);
    }

    return loadGPUProgram(material->shaderType);
}
