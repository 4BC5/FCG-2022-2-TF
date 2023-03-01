#include <iostream>

#include <Window/Window.h>
#include <SceneManager.h>
#include <Nodes/Node3D.h>
#include <Nodes/NodeMesh3D.h>
#include <Renderer/Renderer.h>
#include <Camera.h>

#include <iostream>
#include <matrices.h>
#include <utils.h>

bool running = true;
bool globalTrs = false;
int LR = 0;
int LL = 0;
float rotation = 0.0;

int FW = 0;
int BW = 0;
int L = 0;
int R = 0;

Camera* cam = new Camera();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        running = false;

    glm::vec4 movement = glm::vec4(0.0f);
    if (key == GLFW_KEY_S)
    {
        if (action == GLFW_PRESS)
            BW = 1;
        else if (action == GLFW_RELEASE)
            BW = 0;
    }
    if (key == GLFW_KEY_W)
    {
        if (action == GLFW_PRESS)
            FW = 1;
        else if (action == GLFW_RELEASE)
            FW = 0;
    }
    if (key == GLFW_KEY_A)
    {
        if (action == GLFW_PRESS)
            L = 1;
        else if (action == GLFW_RELEASE)
            L = 0;
    }
    if (key == GLFW_KEY_D)
    {
        if (action == GLFW_PRESS)
            R = 1;
        else if (action == GLFW_RELEASE)
            R = 0;
    }

    if (key == GLFW_KEY_RIGHT)
    {
        if (action == GLFW_PRESS)
            LR = 1;
        else if (action == GLFW_RELEASE)
            LR = 0;
    }
    if (key == GLFW_KEY_LEFT)
    {
        if (action == GLFW_PRESS)
            LL = 1;
        else if (action == GLFW_RELEASE)
            LL = 0;
    }

    if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
        globalTrs = !globalTrs;
    }

    rotation = (LL - LR) * 1.0f;
}

int main()
{
    Window* window = new Window();
    Node3D* sceneRoot = new Node3D();
    NodeMesh3D* cube = new NodeMesh3D("../DreamGliderAssets/Meshes/Trees/Tree01.obj", "");
    NodeMesh3D* cube2 = new NodeMesh3D("../DreamGliderAssets/Meshes/Cube.obj", "");
    std::cout << "Verts: " << cube2->vertices.size() << "\n";
    sceneRoot->addChild(cube2);
    sceneRoot->addChild(cube);
    sceneRoot->addChild(cam);

    cube2->translate(glm::vec3(0.0f,-1.0f,-2.0f));
    cube2->scale(glm::vec3(6.0f,1.0f,6.0f));
    cube->translate(glm::vec3(0.0f,0.0f,-4.0f));
    cam->translate(glm::vec3(0.0f,1.70f,0.0f));

    sceneRoot->root = true;
    cube->name = "Cube";
    cube2->name = "Cube2";
    cam->name = "Camera";

    SceneManager sceneManager(sceneRoot);
    Renderer renderer(window, cam, sceneRoot);

    glfwSetKeyCallback(window->getWindow(), key_callback);


    /*
    cube->vertices = {glm::vec4(-0.5f, -0.5f, -1.0f, 1.0f),
                      glm::vec4(0.5f, -0.5f, -1.0f, 1.0f),
                      glm::vec4(0.5f, 0.5f, -1.0f, 1.0f),};
    cube->triangles = {2,0,1};

    cube->normals = {glm::vec4(0.0f, 0.0f, -1.0f, 0.0f),
                     glm::vec4(0.0f, 0.0f, -1.0f, 0.0f),
                     glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)};

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint g_GpuProgramID = renderer.loadGPUProgram(cube->getShaderPath());

    GLuint verticesVBOID;
    glGenBuffers(1, &verticesVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBOID);//Bind vertices VBO

    glBufferData(GL_ARRAY_BUFFER, cube->vertices.size() * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);//Allocate memory for the vertices VBO on the GPU
    glBufferSubData(GL_ARRAY_BUFFER, 0, cube->vertices.size() * sizeof(glm::vec4), &cube->vertices[0]);//Copy vertices VBO to VRAM memory

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);//Define vertex attribute data
    glEnableVertexAttribArray(0);//Enable vertex attribute data
    glBindBuffer(GL_ARRAY_BUFFER, 0);//Disable VBO

    GLuint indices;
    glGenBuffers(1, &indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube->triangles.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, cube->triangles.size() * sizeof(GLuint), &cube->triangles[0]);

    glUseProgram(g_GpuProgramID);

    while (running)
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, cube->triangles.size(), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window->getWindow());
    }*/

    double startTime = 0.0;
    while (running)
    {
        double deltaTime = glfwGetTime() - startTime;
        glm::vec4 movement = glm::vec4((float)(R - L),0.0f,(float)(BW - FW),0.0f);
        cam->rotateGlobalY(rotation * (float)deltaTime * 10000.0f);
        if (globalTrs)
        {
            cam->globalTranslate(glm::vec3(movement.x, movement.y, movement.z) * (float)deltaTime * 40000.0f);
        }
        else
        {
            cam->translate(glm::vec3(movement.x, movement.y, movement.z) * (float)deltaTime * 40000.0f);
        }
        sceneManager.applyTransforms();
        renderer.render();
        glfwPollEvents();
        glCheckError();
        //mop::PrintMatrix(cam->getTransform());
        startTime = glfwGetTime();
    }

    return 0;
}
