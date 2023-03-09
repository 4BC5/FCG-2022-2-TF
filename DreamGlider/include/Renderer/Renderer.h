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
#include <DirectionalLight.h>
#include <UI/UIElement.h>
#include <Environment.h>

enum s_ShaderType {SHADER_BLINN_PHONG = 0, SHADER_UNSHADED = 1, SHADER_DEPTH = 2, SHADER_BLINN_PHONG_ALPHA_DISCARD = 3, SHADER_DEPTH_ALPHA_DISCARD = 4, SHADER_SHALLOW_WATER = 5, SHADER_DEPTH_RENDER = 6};

const std::vector<std::string> shaderPaths = {"src/Shaders/Blinn-Phong/main", "src/Shaders/unsh", "src/Shaders/Depth/depth_shader", "src/Shaders/Blinn-Phong/bp_alpha_discard", "src/Shaders/Depth/depth_shader_discard", "src/Shaders/Water/shallow_water", "src/Shaders/Depth/depthRender"};

typedef struct _shader
{
    int shaderType;
    GLuint programID;
} Shader;


/*const char BLINN_PHONG_PATH[] = "src/Shaders/main";
const char UNSHADED_PATH[] = "src/Shaders/unsh";
const char DEPTH_SHADER_PATH[] = "src/Shaders/depth_shader";*/

class Renderer
{
    public:
        Renderer(Window* window, Camera* cam, Node* root);
        virtual ~Renderer();

        std::vector<Shader> loadedshaders = {};
        std::vector<Texture> loadedTextures = {};

        //GLuint getShader;
        void render();
        void setDirectionalLight(DirectionalLight* directionalLight){this->directionalLight = directionalLight;}
        void setEnvironment(Environment* env){environment = env;}
    protected:

    private:
        void renderObject(Node* object);
        void renderShadowMap();
        void setUpShadowMapping();
        void renderShadowMapRec(Node* object, int index);
        void renderGUI();

        DirectionalLight* directionalLight = nullptr;
        Node* sceneRoot;
        Camera* camera;
        Window* window;
        Environment* environment = nullptr;

        GLuint loadTexture(std::string path);
        GLuint loadMaterial(Material* material);
        GLuint loadGPUProgram(int shaderType);
        GLuint buildMesh(NodeMesh3D* meshNode);

        GLuint depthProgram;
        GLuint depthDiscardProgram;
        //GLuint loadGPUProgram(std::string path);

};

#endif // RENDERER_H
