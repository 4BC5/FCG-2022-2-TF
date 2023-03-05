#include <iostream>

#include <Window/Window.h>
#include <SceneManager.h>
#include <Nodes/Node3D.h>
#include <Nodes/NodeMesh3D.h>
#include <Renderer/Renderer.h>
#include <Camera.h>
#include <Material.h>

#include <iostream>
#include <matrices.h>
#include <utils.h>
#include <chrono>
#include <thread>
#include <cmath>

bool running = true;
bool globalTrs = false;
int LR = 0;
int LL = 0;
float rotation = 0.0;

int FW = 0;
int BW = 0;
int L = 0;
int R = 0;

Camera* cam = new Camera("camera", 0.05, 300.0, 0.0);
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

float lerp(float from, float to, float alpha)
{
    return (from + (to - from) * alpha);
}

float lengthSquared(glm::vec4 vec)
{
    return vec.x * vec.x + vec.y * vec.y + vec.z + vec.z;
}

float deg2rad(float deg)
{
    return (deg*3.141592f)/180.0f;
}

int main()
{
    Material* defaultMat = new Material(glm::vec4(0.5f));
    Material* wood = new Material("../DreamGliderAssets/Materials/MossyTreeBark/MossyTreeBark_albedo.png", "../DreamGliderAssets/Materials/MossyTreeBark/MossyTreeBark_normal.png");
    Material* grass = new Material("../DreamGliderAssets/Materials/Grass/Grass_albedo.png","../DreamGliderAssets/Materials/Grass/Grass_normal.png");
    Material* leaves = new Material("../DreamGliderAssets/Materials/Leaves/Leaves_albedo.png");
    //Material* terrain = new Material("")

    wood->normalStrength = 0.8;
    grass->UVtiling = glm::vec2(20.0f);
    leaves->shaderType = SHADER_BLINN_PHONG_ALPHA_DISCARD;
    leaves->faceCulling = false;


    Window* window = new Window();

    Node3D* sceneRoot = new Node3D("scene root");
    Node3D* player = new Node3D("player");

    NodeMesh3D* tree = new NodeMesh3D("Tree" ,"../DreamGliderAssets/Meshes/Trees/Tree01.obj", wood);
    NodeMesh3D* treeLeaves = new NodeMesh3D("Leaves", "../DreamGliderAssets/Meshes/Trees/Tree01Leaves.obj", leaves);
    NodeMesh3D* tree2 = new NodeMesh3D("Tree2" ,"../DreamGliderAssets/Meshes/Trees/Tree01.obj", wood);
    NodeMesh3D* tree2Leaves = new NodeMesh3D("Leaves", "../DreamGliderAssets/Meshes/Trees/Tree01Leaves.obj", leaves);
    NodeMesh3D* screen = new NodeMesh3D( "Screen" ,"../DreamGliderAssets/Meshes/Screen.obj", wood);
    NodeMesh3D* pondIsland = new NodeMesh3D("Pond island", "../DreamGliderAssets/Meshes/Islands/PondIsland.obj", grass);
    NodeMesh3D* pond = new NodeMesh3D("Pond", "../DreamGliderAssets/Meshes/Islands/Pond.obj", defaultMat);
    NodeMesh3D* buny = new NodeMesh3D( "Buny" ,"../DreamGliderAssets/Meshes/bunny.obj", defaultMat);

    Camera* sun = new Camera("SUN", 0.1, 200.0, 0.0);

   // sceneRoot->addChild(buny);

    sceneRoot->addChild(tree);
    sceneRoot->addChild(tree2);
    sceneRoot->addChild(player);
    sceneRoot->addChild(pondIsland);
    pondIsland->addChild(buny);

    tree->addChild(treeLeaves);
    tree2->addChild(tree2Leaves);

    player->addChild(cam);
    cam->addChild(sun);

    sceneRoot->addChild(screen);

    screen->translate(glm::vec3(0.0f,1.0f,-8.0f));
    pondIsland->addChild(pond);
    sun->translate(glm::vec3(0.0f,0.0f,-10.0f));
    sun->translate(glm::vec3(0.0f,80.0f,0.0f));
    sun->rotateGlobalX(-3.141592f/2.0f);
    screen->rotateGlobalX(-3.141592f/4.0f);
    tree->translate(glm::vec3(14.0f,1.0f,0.0f));
    tree2->translate(glm::vec3(0.0f,-0.7f,2.0f));
    tree->rotateGlobalY(deg2rad(60.0f));
    tree2->rotateGlobalY(deg2rad(180.0f));
    player->translate(glm::vec3(0.0f,1.70f,2.0f));
 //    buny->translate(glm::vec3(2.0f,2.0f,0.0f));

    //std::cout << "SUN TRS: \n";
    //mop::PrintMatrix(sun->getTransform());

    sceneRoot->root = true;

    SceneManager sceneManager(sceneRoot);
    Renderer renderer(window, cam, sceneRoot, sun);

    glfwSetKeyCallback(window->getWindow(), key_callback);

    glm::vec4 velocity = glm::vec4(0.0f);
    const float acceleration = 80.0f;

    float rotationVelocity = 0.0f;

    float deltaTime = 0.015899;
    double startTime = glfwGetTime();
    while (running)
    {
        startTime = glfwGetTime();
        double tickStart = glfwGetTime();


        glm::vec4 movement = (float)(R - L) * cam->getGlobalBasisX() + (float)(BW - FW) * cam->getGlobalBasisZ();
        movement = glm::length(movement) < 1.0f ? movement : glm::normalize(movement);
        velocity -= velocity * 12.0f * deltaTime;
        velocity += movement * acceleration * deltaTime;
        player->translate(glm::vec3(velocity.x, velocity.y, velocity.z) * (float)deltaTime);

        rotationVelocity = lerp(rotationVelocity, rotation, 16.0f * deltaTime);
        cam->rotateGlobalY(rotationVelocity * (float)deltaTime * 3.0f);

        sceneManager.applyTransforms();
        renderer.render();
        glfwPollEvents();
        //mop::PrintMatrix(cam->getTransform());
        double remainder = 0.015899 - (glfwGetTime() - tickStart);
        while(remainder > 0.0001)
        {
            remainder = 0.015899 - (glfwGetTime() - tickStart);
        }
        deltaTime = glfwGetTime() - startTime;
    }
    mop::PrintMatrix(sun->getTransform());

    return 0;
}
