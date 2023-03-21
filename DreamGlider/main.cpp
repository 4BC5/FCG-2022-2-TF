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
#include <Environment.h>
#include <PhysicsBody.h>
#include <CollisionShape.h>
#include <Player.h>
#include <WindTube.h>
#include <TriggerVolume.h>

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

//Variáveis de transformação
bool Trans = false;
bool Trans_inv = false;
bool rot = false;

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

bool jump = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //ESC: Terminação do programa
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        running = false;

    //WASD: Movimento
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

    //Transformações pelo usuário:
    if (key == GLFW_KEY_B)
    {
        if (action == GLFW_PRESS)
            Trans = true;
        else if (action == GLFW_RELEASE)
            Trans = false;
    }
    if (key == GLFW_KEY_R)
    {
        if (action == GLFW_PRESS)
            rot = true;
        else //if (action == GLFW_RELEASE)
            rot = false;

    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        jump = true;
    }
}

//Função de Transfomação pelo usuário
void transformation(NodeMesh3D* object, char type)
{
    switch (type)
    {
    case 'R': //Rotate
        if(rot)
            object->rotateLocalX(M_PI/4);
        break;
    case 'S': //Scale 2x
        if (Trans && !Trans_inv)
        {
            object->scale(glm::vec3(2.0f,2.0f,2.0f));
            Trans_inv = true;
        }
        else if (Trans_inv && !Trans)
        {
            object->scale(glm::vec3(0.5f,0.5f,0.5f));
            Trans_inv = false;
        }
        break;
    default:
        break;
    }
}

//Função de construção de curva de Bezier
void curvature(NodeMesh3D* object, Curves* curva, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d)
{
    if (a!=b) //Se os dois 1os pontos forem iguais, faz a curva default
    {
        curva->A = a;
        curva->B = b;
        curva->C = c;
        curva->D = d;
    }
    object->setPosition(curva->interpolateTime( abs( sin( glfwGetTime() ) ) ));
}

PhysicsBody* addTree(Mesh3D* trunkMesh, Mesh3D* leavesMesh, Mesh3D* trunkCollisionMesh, Material* trunkMaterial, Material* leavesMaterial)
{
    PhysicsBody* trunkBody = new PhysicsBody("trunkBody", PHYS_BODY_STATIC);
    CollisionShape* trunkCollision = new CollisionShape("trunkCol");
    trunkCollision->setCollisionType(COLLISION_TRIANGLE);
    trunkCollision->setMesh(trunkCollisionMesh);
    trunkBody->addChild(trunkCollision);

    NodeMesh3D* trunk3DMesh = new NodeMesh3D("trunkMesh", trunkMesh, trunkMaterial);
    NodeMesh3D* leaves3DMesh = new NodeMesh3D("leavesMesh", leavesMesh, leavesMaterial);
    trunkBody->addChild(trunk3DMesh);
    trunkBody->addChild(leaves3DMesh);
    return trunkBody;
}

int main()
{
    Node3D* sceneRoot = new Node3D("scene root");
    Camera* cam = new Camera("camera", 0.2, 2000.0, 0.0);

    Window* window = new Window();
    Renderer renderer(window, cam, sceneRoot);

    Environment* env = new Environment("../DreamGliderAssets/Cubemaps/Sea/sea");
    renderer.setEnvironment(env);
    Material::initializeDefaultTextures();
    SceneManager sceneManager(sceneRoot);
    Node::setSceneManager(&sceneManager);
    //Materials
    Material* defaultMat = new Material(glm::vec4(0.5f));
    defaultMat->setShaderType(SHADER_PBR);
    defaultMat->setColor(glm::vec4(1.0f,0.7f,0.0f,0.0f));
    defaultMat->setRoughness(0.4);
    defaultMat->setMetallic(1.0);


    Mesh3D* bunnyMesh = new Mesh3D("../DreamGliderAssets/Meshes/bunny.obj");
    //Objetos
    NodeMesh3D* buny = new NodeMesh3D( "Buny" , bunnyMesh, defaultMat);



    Node3D* rotationTex = new Node3D("RTS");

    //Inicialização de cena


    //Curva Bezier
    Curves* trajeto = new Curves("trajeto");


    //Sol
    glCheckError();
    DirectionalLight* sun = new DirectionalLight("SUN");
    sun->setColor(glm::vec4(1.0f,0.0f,0.0f,1.0f));
    sun->setShadowsEnabled(true);
    sun->setShadowResolution(2048);
    sun->setNumShadowSamples(8);
    glCheckError();

    //Setup de cena (Adicionar objetos)
    sceneRoot->addChild(buny);
    sceneRoot->addChild(rotationTex);
    rotationTex->addChild(sun);

    //Player
    Player* playerTest = new Player("player", cam);
    CollisionShape* playerCol = new CollisionShape("playerCol");
    playerCol->setCollisionType(COLLISION_SPHERE);
    playerCol->setRadius(1.0f);
    playerTest->addChild(playerCol);
    playerTest->translate(glm::vec3(0.0f,60.0f,0.0f));

    Node3D* camY = new Node3D("camY");
    camY->translate(glm::vec3(0.0f,0.70f,0.0f));

    camY->addChild(cam);
    playerTest->addChild(camY);

    sceneRoot->addChild(playerTest);

    //Setup de cena (Organizar objetos)
    WindTube* windTube = new WindTube("windTube");
    CollisionShape* windTubeCol = new CollisionShape("windCol");
    TriggerVolume* windTrigger = new TriggerVolume("windTrigger");
    windTubeCol->setCollisionType(COLLISION_SPHERE);
    windTubeCol->setRadius(16.0f);
    windTube->addChild(windTrigger);
    windTrigger->addChild(windTubeCol);
    windTrigger->setOnCollisionReceiver(static_cast<Node*>(windTube));
    sceneRoot->addChild(windTube);
    windTube->translate(glm::vec3(16.0f, 8.0f, 0.0f));

    rotationTex->translate(glm::vec3(0.0f,1.0f,0.0f));
    rotationTex->rotateGlobalX(3.141592f/12.0f);

    buny->translate(glm::vec3(0.0f,1.4f,0.0f));

    Node* impSceneNode = sceneManager.loadSceneFromFile("../DreamGliderAssets/Scenes/Islands.scn");
    sceneRoot->addChild(impSceneNode);

    sceneRoot->root = true;

    //Gerenciamento e Renderização
    glfwSetKeyCallback(window->getWindow(), key_callback);
    glfwSetCursorPosCallback(window->getWindow(), mouse_callback);
    glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window->getWindow(), 0.0, 0.0);
    glCheckError();

    renderer.setDirectionalLight(sun);

    //Movimento
    float xRot = 0.0f;


    float deltaTime = 0.015899;
    double startTime;

    int jFrames = 0;
    //Laço de Execução
    sceneManager.applyTransforms();
    while (!glfwWindowShouldClose(window->getWindow()))
    {
        startTime = glfwGetTime();
        double tickStart = glfwGetTime();


        glm::vec4 movement = float(R - L) * camY->getGlobalBasisX() + float(BW - FW) * camY->getGlobalBasisZ();// + float(UP - DOWN) * camY->getBasisY();
        movement = glm::length(movement) < 1.0f ? movement : glm::normalize(movement);
        playerTest->addAcceleration(movement * 160.0f);
        xRot = clamp(xRot + mouseDeltaY * 0.0025,-3.141592f/2.0f, 3.141592f/2.0f);
        camY->rotateGlobalY(mouseDeltaX * 0.0025);
        cam->resetRotation();
        cam->rotateLocalX(xRot);
        if (jump)
        {
            playerTest->jump();
            if (jFrames < 4)
            {
                jFrames++;
            }
            else
            {
                jump = false;
                jFrames = 0;
            }
        }

        //Transformação pelo usuário
        transformation(buny, 'S');
        transformation(buny, 'R');

        //Curva de Bezier
        //curvature(buny,trajeto,glm::vec3(0.0f,1.4f,0.0f),glm::vec3(0.0f,1.4f,0.0f),glm::vec3(0.0f,1.4f,0.0f),glm::vec3(0.0f,1.4f,0.0f));
        curvature(buny,trajeto,glm::vec3(0.0f,1.4f,0.0f),glm::vec3(0.0f,3.0f,0.0f),glm::vec3(2.0f,1.7f,0.0f),glm::vec3(2.0f,1.4f,0.0f));

        sun->rotateGlobalY(deltaTime * 0.2);

        //Sempre mover objetos antes de apply transform
        sceneManager.applyPhysics(deltaTime);
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
