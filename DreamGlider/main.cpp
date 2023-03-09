#include <iostream>

#include <Window/Window.h>
#include <SceneManager.h>
#include <Nodes/Node3D.h>
#include <Nodes/NodeMesh3D.h>
#include <Renderer/Renderer.h>
#include <UI/UIElement.h>
#include <UI/UIText.h>
#include <Nodes/Curves.h>
#include <Camera.h>
#include <Material.h>
#include <DirectionalLight.h>
#include <Mesh3D.h>


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

int UP = 0;
int DOWN = 0;

double mousePosX = 0.0f;
double mousePosY = 0.0f;
float mouseDeltaX = 0.0f;
float mouseDeltaY = 0.0f;

float acceleration = 80.0f;

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

float clamp(float value, float low, float high)
{
    const float t = value < low ? low : value;
    return t > high ? high : t;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouseDeltaX = mousePosX - xpos;
    mouseDeltaY = mousePosY - ypos;
    mousePosX = xpos;
    mousePosY = ypos;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //ESC: Terminação do programa
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        running = false;

    //WASD: Movimento
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
    if (key == GLFW_KEY_Q)
    {
        if (action == GLFW_PRESS)
            DOWN = 1;
        else if (action == GLFW_RELEASE)
            DOWN = 0;
    }

    if (key == GLFW_KEY_E)
    {
        if (action == GLFW_PRESS)
            UP = 1;
        else if (action == GLFW_RELEASE)
            UP = 0;
    }

    rotation = (LL - LR) * 1.0f;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    acceleration = clamp(acceleration + yoffset * 10.0f, 10.0f, 1000.0f);
}

int main()
{
    //Materials
    Material* defaultMat = new Material(glm::vec4(0.5f));
    Material* dr = new Material(glm::vec4(1.0f));
    dr->shaderType = SHADER_DEPTH_RENDER;
    Material* wood = new Material("../DreamGliderAssets/Materials/MossyTreeBark/MossyTreeBark_albedo.png", "../DreamGliderAssets/Materials/MossyTreeBark/MossyTreeBark_normal.png");
    Material* grass = new Material("../DreamGliderAssets/Materials/Grass/Grass_albedo.png","../DreamGliderAssets/Materials/Grass/Grass_normal.png");
    Material* leaves = new Material("../DreamGliderAssets/Materials/Leaves/Leaves_albedo.png");

    leaves->transmission = 0.8f;
    //Material* terrain = new Material("")

    wood->normalStrength = 0.8;
    grass->UVtiling = glm::vec2(20.0f);
    leaves->shaderType = SHADER_BLINN_PHONG_ALPHA_DISCARD;
    leaves->faceCulling = false;

    Mesh3D* treeTrunk = new Mesh3D("../DreamGliderAssets/Meshes/Trees/Tree01.obj");
    Mesh3D* leavesMesh = new Mesh3D("../DreamGliderAssets/Meshes/Trees/Tree01Leaves.obj");
    Mesh3D* screenMesh = new Mesh3D("../DreamGliderAssets/Meshes/Screen.obj");
    Mesh3D* pondIslandMesh = new Mesh3D("../DreamGliderAssets/Meshes/Islands/PondIsland.obj");
    Mesh3D* pondMesh = new Mesh3D("../DreamGliderAssets/Meshes/Islands/Pond.obj");
    Mesh3D* bunnyMesh = new Mesh3D("../DreamGliderAssets/Meshes/bunny.obj");
    Mesh3D* cubeMesh = new Mesh3D("../DreamGliderAssets/Meshes/Cube.obj");


    //Objetos
    NodeMesh3D* tree = new NodeMesh3D("Tree" , treeTrunk, wood);
    NodeMesh3D* treeLeaves = new NodeMesh3D("Leaves", leavesMesh, leaves);
    NodeMesh3D* tree2 = new NodeMesh3D("Tree2" , treeTrunk, wood);
    NodeMesh3D* tree2Leaves = new NodeMesh3D("Leaves", leavesMesh, leaves);
    NodeMesh3D* screen = new NodeMesh3D( "Screen" , screenMesh, dr);
    NodeMesh3D* plane = new NodeMesh3D( "plane" , screenMesh, defaultMat);
    NodeMesh3D* pondIsland = new NodeMesh3D("Pond island", pondIslandMesh, grass);
    NodeMesh3D* pond = new NodeMesh3D("Pond", pondMesh, defaultMat);
    NodeMesh3D* buny = new NodeMesh3D( "Buny" , bunnyMesh, defaultMat);
    NodeMesh3D* cube = new NodeMesh3D( "Cube", cubeMesh, defaultMat);
    NodeMesh3D* bushCube = new NodeMesh3D("bush cube", cubeMesh, leaves);

    Node3D* rotationTex = new Node3D("RTS");

    //Câmera
    Camera* cam = new Camera("camera", 0.05, 300.0, 0.0);

    //Inicialização de cena
    Node3D* sceneRoot = new Node3D("scene root");
    Node3D* player = new Node3D("player");

    //Janela
    Window* window = new Window();
    Renderer renderer(window, cam, sceneRoot);


    //Curva Bezier
    Curves* trajeto = new Curves("trajeto");


    //Sol
    DirectionalLight* sun = new DirectionalLight("SUN");
    sun->setShadowsEnabled(true);
    sun->setShadowResolution(2048);
    sun->addChild(cube);

    //Setup de cena (Adicionar objetos)

    sceneRoot->addChild(buny);
    sceneRoot->addChild(tree);
    sceneRoot->addChild(tree2);
    sceneRoot->addChild(player);
    sceneRoot->addChild(pondIsland);
    sceneRoot->addChild(plane);
    sceneRoot->addChild(rotationTex);
    sceneRoot->addChild(bushCube);
    rotationTex->addChild(sun);
    pondIsland->addChild(buny);

    tree->addChild(treeLeaves);
    tree2->addChild(tree2Leaves);

    player->addChild(cam);
    cam->addChild(screen);

    //Setup de cena (Organizar objetos)
    bushCube->translate(glm::vec3(0.0f,1.0f,-1.0f));

    cube->scale(glm::vec3(0.25));
    rotationTex->translate(glm::vec3(0.0f,1.0f,0.0f));
    plane->rotateGlobalX(-3.141592f/2.0f);
    plane->scale(glm::vec3(30.0f));
    plane->translate(glm::vec3(0.0f,-20.0f,0.0f));

    screen->translate(glm::vec3(0.3f,0.16f,-0.26f));
    screen->scale(glm::vec3(0.1));
    buny->translate(glm::vec3(0.0f,1.4f,0.0f));
    pondIsland->addChild(pond);
    tree->translate(glm::vec3(14.0f,1.0f,0.0f));
    tree2->translate(glm::vec3(0.0f,-0.7f,2.0f));
    tree->rotateGlobalY(deg2rad(60.0f));
    tree2->rotateGlobalY(deg2rad(180.0f));
    player->translate(glm::vec3(0.0f,1.70f,2.0f));
 //    buny->translate(glm::vec3(2.0f,2.0f,0.0f));

    rotationTex->rotateGlobalX(3.141592f/4.0f);
    sceneRoot->root = true;

    //Gerenciamento e Renderização
    SceneManager sceneManager(sceneRoot);

    glfwSetKeyCallback(window->getWindow(), key_callback);
    glfwSetCursorPosCallback(window->getWindow(), mouse_callback);
    glfwSetScrollCallback(window->getWindow(), scroll_callback);
    glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    renderer.setDirectionalLight(sun);

    //Movimento
    glm::vec4 velocity = glm::vec4(0.0f);
    float xRot = 0.0f;


    float deltaTime = 0.015899;
    double startTime;

    //Laço de Execução
    while (running)
    {
        startTime = glfwGetTime();
        double tickStart = glfwGetTime();


        glm::vec4 movement = float(R - L) * cam->getGlobalBasisX() + float(BW - FW) * cam->getGlobalBasisZ() + float(UP - DOWN) * player->getBasisY();
        movement = glm::length(movement) < 1.0f ? movement : glm::normalize(movement);
        velocity -= velocity * 12.0f * deltaTime;
        velocity += movement * acceleration * deltaTime;
        player->translate(glm::vec3(velocity.x, velocity.y, velocity.z) * (float)deltaTime);

        xRot = clamp(xRot + mouseDeltaY * 0.0025,-3.141592f/2.0f, 3.141592f/2.0f);
        player->rotateGlobalY(mouseDeltaX * 0.0025);
        cam->resetRotation();
        cam->rotateLocalX(xRot);


        //Objeto em movimento: buny
        buny->setPosition(trajeto->interpolateTime(abs(sin(startTime))));
        sun->rotateGlobalY(deltaTime * 0.2);
        //Sempre mover objetos antes de apply transform
        sceneManager.applyTransforms();
        renderer.render();
        mouseDeltaX = 0.0f;
        mouseDeltaY = 0.0f;
        glfwPollEvents();
        double remainder = 0.015899 - (glfwGetTime() - tickStart);
        while(remainder > 0.0001)
        {
            remainder = 0.015899 - (glfwGetTime() - tickStart);
        }
        deltaTime = glfwGetTime() - startTime;
    }

    return 0;
}
