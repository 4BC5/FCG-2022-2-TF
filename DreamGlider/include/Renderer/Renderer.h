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

enum s_ShaderType {SHADER_BLINN_PHONG = 0, SHADER_UNSHADED = 1, SHADER_DEPTH = 2, SHADER_BLINN_PHONG_ALPHA_DISCARD = 3, SHADER_DEPTH_ALPHA_DISCARD = 4, SHADER_SHALLOW_WATER = 5, SHADER_DEPTH_RENDER = 6, SHADER_PBR = 7, SHADER_PBR_ALPHA_DISCARD = 8, SHADER_OCEAN = 9};

const std::vector<std::string> shaderPaths = {"Shaders/Blinn-Phong/main", "Shaders/unsh", "Shaders/Depth/depth_shader", "Shaders/Blinn-Phong/bp_alpha_discard", "Shaders/Depth/depth_shader_discard", "Shaders/Water/shallow_water", "Shaders/Depth/depthRender", "Shaders/PBR/pbr", "Shaders/PBR/pbr_alphaDiscard", "Shaders/Water/ocean"};

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

        std::vector<GLuint> loadedshaders = {};
        std::vector<Texture> loadedTextures = {};

        //GLuint getShader;
        void render();
        void setDirectionalLight(DirectionalLight* directionalLight){this->directionalLight = directionalLight;}
        void setEnvironment(Environment* env){environment = env;}
    protected:

    private:
        void renderObject(Node* object);
        void renderShadowMap();
        void renderTransparentObjects();

        void setUpShadowMapping();
        void renderShadowMapRec(Node* object, int index);
        void renderGUI();
        void createUBOs();
        void updateMatricesUBO();
        void updateDirectionalLightUBO();
        void loadAllShaders();

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

        GLuint matricesUBO = 0;
        GLuint directionalLightUBO = 0;
        GLuint shadowsUBO = 0;
        //GLuint loadGPUProgram(std::string path);

        std::vector<NodeMesh3D*> transparentObjects = {};

};

#endif // RENDERER_H
