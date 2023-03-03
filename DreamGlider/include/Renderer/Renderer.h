#ifndef RENDERER_H
#define RENDERER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <Nodes/Node.h>
#include <Renderer/Camera/Camera.h>
#include <glm/mat4x4.hpp>
#include <Window/Window.h>
#include <glm/vec4.hpp>
#include <fstream>
#include <matrices.h>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <Nodes/NodeMesh3D.h>
#include <Material.h>
#include <utils.h>
#include <stb_image.h>

#define BLINN_PHONG_PATH "src/Shaders/main"
#define UNSHADED_PATH "src/Shaders/unsh"
#define DEPTH_SHADER_PATH "src/Shaders/depth_shader"

enum s_ShaderType {SHADER_BLINN_PHONG = 0, SHADER_UNSHADED = 1, SHADER_DEPTH = 2};

typedef struct _shader
{
    s_ShaderType shaderType;
    GLuint programID;
} Shader;

typedef struct _texture
{
    std::string texturePath;
    GLuint textureId;
} Texture;


/*const char BLINN_PHONG_PATH[] = "src/Shaders/main";
const char UNSHADED_PATH[] = "src/Shaders/unsh";
const char DEPTH_SHADER_PATH[] = "src/Shaders/depth_shader";*/

class Renderer
{
    public:
        Renderer(Window* window, Camera* cam, Node* root, Camera* directional);
        virtual ~Renderer();

        std::vector<Shader> loadedshaders = {};
        std::vector<Texture> loadedTextures = {};

        Node* sceneRoot;
        Camera* camera;
        Camera* directional;
        Window* window;
        GLuint depthMapFBO;
        GLuint depthMap;
        GLuint getShader;
        GLuint depthProgram;
        void render();
    protected:

    private:
        void renderObject(Node* object);
        void renderShadowMap(Node* object);
        void setUpShadowMap();
        GLuint loadTexture(std::string path);
        GLuint loadMaterial(Material* material);
        GLuint loadGPUProgram(int shaderType);
        GLuint buildMesh(NodeMesh3D* meshNode);
        //GLuint loadGPUProgram(std::string path);

};

#endif // RENDERER_H
