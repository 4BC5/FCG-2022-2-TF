#include "Renderer.h"
#define L_VERTICES 0
#define L_UVS 1
#define L_NORMALS 2
#define D_VERTICES 4
#define D_UVS 2
#define D_NORMALS 4

std::vector<Shader> Renderer::shaders;

Renderer::Renderer(Window* window, Camera* cam, Node* root)
{
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    sceneRoot = root;
    this->window = window;
    camera = cam;
}

Renderer::~Renderer()
{
    //dtor
}

void Renderer::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Clear depth buffer
    glEnable(GL_DEPTH_TEST);//Enable depth buffer

    renderObject(sceneRoot);

    glfwSwapBuffers(window->getWindow());
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
        GLuint g_GpuProgramID = loadGPUProgram(object->getShaderPath());//Load GPU program
        GLuint VAOId = buildMesh(object);//Build VAO

        glBindVertexArray(VAOId);//Bind VAO

        glUseProgram(g_GpuProgramID);//Set GPU program handle to use
        GLint modelUniform           = glGetUniformLocation(g_GpuProgramID, "model"); // Variável da matriz "model"
        GLint viewUniform            = glGetUniformLocation(g_GpuProgramID, "view"); // Variável da matriz "view" em shader_vertex.glsl
        GLint projectionUniform      = glGetUniformLocation(g_GpuProgramID, "projection");

        glm::mat4 perspective = mop::Matrix_Perspective(camera->getFOV(),window->getAspect(),camera->getNearPlane(),camera->getFarPlane());
        glm::mat4 view = mop::Matrix_Camera_View(camera->getPosition(), -camera->getBasisZ(), camera->upVector);
        glm::mat4 globalTransform = object->getGlobalTransform();

        mop::PrintMatrix(globalTransform);

        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(globalTransform));
        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(perspective));

        glDrawElements(GL_TRIANGLES, object->getMeshTriangles().size(), GL_UNSIGNED_INT, &object->getMeshTriangles()[0]);

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

GLuint Renderer::buildMesh(Node* object)
{
    std::vector<glm::vec4> meshVertices = object->getMeshPoints();
    std::vector<glm::vec4> meshNormals = object->getMeshNormals();
    std::vector<glm::vec2> meshUVs = object->getMeshUVs();
    std::vector<GLuint> meshTriangles = object->getMeshTriangles();

    GLuint verticesVBOID;
    glGenBuffers(1, &verticesVBOID);

    GLuint VAOId;
    glGenVertexArrays(1, &VAOId);//Generate VAO id
    glBindVertexArray(VAOId);//Bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBOID);//Bind vertices VBO

    glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);//Allocate memory for the vertices VBO on the GPU
    glBufferSubData(GL_ARRAY_BUFFER, 0, meshVertices.size() * sizeof(glm::vec4), &meshVertices[0]);//Copy vertices VBO to VRAM memory

    glVertexAttribPointer(L_VERTICES, D_VERTICES, GL_FLOAT, GL_FALSE, 0, 0);//Define vertex attribute data
    glEnableVertexAttribArray(L_VERTICES);//Enable vertex attribute data
    glBindBuffer(GL_ARRAY_BUFFER, 0);//Disable VBO


    //////////////////////////////////////// UVs


    GLuint UVsVBOId;
    glGenBuffers(1, &UVsVBOId);
    glBindBuffer(GL_ARRAY_BUFFER, UVsVBOId);
    glBufferData(GL_ARRAY_BUFFER, meshUVs.size() * sizeof(glm::vec2), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, meshUVs.size() * sizeof(glm::vec2), &meshUVs[0]);
    glVertexAttribPointer(L_UVS, D_UVS, GL_FLOAT, GL_FALSE, 0, 0);//Position 1, 2 members
    glEnableVertexAttribArray(L_UVS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ///////////////////////////////////// Normals

    GLuint normalsVBOId;
    glGenBuffers(1, &normalsVBOId);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBOId);
    glBufferData(GL_ARRAY_BUFFER, meshNormals.size() * sizeof(glm::vec4), &meshNormals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(L_NORMALS, D_NORMALS, GL_FLOAT, GL_TRUE, meshNormals.size() * sizeof(glm::vec4), &meshNormals[0]);
    glEnableVertexAttribArray(L_NORMALS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ///////////////////////////////////////Triangles

    GLuint indices;
    glGenBuffers(1, &indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshTriangles.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, meshTriangles.size() * sizeof(GLuint), &meshTriangles[0]);

    glBindVertexArray(0);
    return VAOId;
}

bool loadShader(std::string shaderPath, GLuint shaderID)
{
    std::ifstream shaderFile(shaderPath, std::ios::in);
    if (!shaderFile.is_open())
    {
        std::cout << "Could not open shader file\n";
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

GLuint Renderer::loadGPUProgram(std::string path)
{
    for (unsigned int i = 0; i < shaders.size(); i++)
    {
        if (shaders[i].path.compare(path))
        {
            return shaders[i].programID;
        }
    }

    GLuint vertexShaderId = loadVertexShader(path);
    GLuint fragmentShaderId = loadFragmentShader(path);

    GLuint GPUProgramId;

    GPUProgramId = CreateGpuProgram(vertexShaderId, fragmentShaderId);
    return GPUProgramId;
}

