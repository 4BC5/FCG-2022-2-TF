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

typedef struct _shader
{
    std::string path;
    GLuint programID;
} Shader;

class Renderer
{
    public:
        Renderer(Window* window, Camera* cam, Node* root);
        virtual ~Renderer();

        static std::vector<Shader> shaders;
        Node* sceneRoot;
        Camera* camera;
        Window* window;

        void render();
    protected:

    private:
        void renderObject(Node* object);
        GLuint buildMesh(Node* object);
        GLuint loadGPUProgram(std::string path);

};

#endif // RENDERER_H
