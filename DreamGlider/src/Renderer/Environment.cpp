#include "Environment.h"

const float cubeVertices[] =
        {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

Environment::Environment(std::string cubemapPath)
{
    loadCubemap(cubemapPath);
    buildCube();
}

Environment::~Environment()
{
    //dtor
}

GLuint loadGPUProgram(std::string path);

void Environment::loadCubemap(std::string cubemapPath)
{
    std::vector<std::string> cubePaths = {cubemapPath + "_right.png", cubemapPath + "_left.png", cubemapPath + "_top.png", cubemapPath + "_bottom.png", cubemapPath + "_back.png", cubemapPath + "_front.png"};

    glGenTextures(1, &cubemapId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);

    unsigned char *data;
    int width, height, channelN;
    for (int i = 0; i < 6; i++)
    {
        data = stbi_load(cubePaths[i].c_str(), &width, &height, &channelN, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    program = loadGPUProgram("Shaders/Environment/environment");

}

void Environment::sendCubemapTexture(GLuint program)
{
    GLint cubemapUniform = glGetUniformLocation(program, "environmentCubemap");
    GLint envStrengthUniform = glGetUniformLocation(program, "environmentStrength");
    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);
    glUniform1i(cubemapUniform, 11);
    glUniform1f(envStrengthUniform, environmentStrength);
}

void Environment::buildCube()
{
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    GLuint vertexVBO;
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 216, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 216, cubeVertices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool Environment::loadShader(std::string shaderPath, GLuint shaderID)
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

GLuint Environment::loadVertexShader(std::string shaderPath)
{
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);

    loadShader(shaderPath + "_vertex.glsl", vertexShaderId);

    return vertexShaderId;
}

GLuint Environment::loadFragmentShader(std::string shaderPath)
{
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    loadShader(shaderPath + "_fragment.glsl", fragmentShaderId);

    return fragmentShaderId;
}

GLuint Environment::CreateGpuProgram(GLuint vertexShaderId, GLuint fragmentShaderId)
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



GLuint Environment::loadGPUProgram(std::string path)
{
    GLuint vertexShaderId = loadVertexShader(path);
    GLuint fragmentShaderId = loadFragmentShader(path);
    GLuint GPUProgramId;

    GPUProgramId = CreateGpuProgram(vertexShaderId, fragmentShaderId);
    return GPUProgramId;
}

void Environment::renderCube(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    //glDisable(GL_CULL_FACE);
    glBindVertexArray(cubeVAO);

    glUseProgram(program);

    GLuint projectionUniform = glGetUniformLocation(program, "projection");
    GLuint viewUniform = glGetUniformLocation(program, "view");
    //GLuint cubemapUniform = glGetUniformLocation(program, "cubemap");
    GLuint strengthUniform = glGetUniformLocation(program, "environmentStrength");

    glm::mat4 view = glm::mat4(glm::mat3(viewMatrix));
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniform1f(strengthUniform, environmentStrength);

    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);
    //glUniform1i(cubemapUniform, 11);


    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}
