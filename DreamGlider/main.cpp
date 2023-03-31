#include <iostream>

#include <Window/Window.h>
#include <SceneManager.h>
#include <Nodes/Node3D.h>
#include <Nodes/NodeMesh3D.h>
#include <Renderer/Renderer.h>
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
#include <SpawnPoint.h>
#include <PointLight.h>
#include <Collectible.h>

#include <iostream>
#include <matrices.h>
#include <utils.h>
#include <chrono>
#include <thread>
#include <cmath>

//Variável Global - Terminação do programa
bool running = true;

//Variáveis Globais - Movimentação
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
bool jump = false;

//Variáveis Globais - Transformação
bool Trans = false;
bool Trans_inv = false;
bool rot = false;

//Função de conversão graus-radianos, utilizada para rotações locais
float deg2rad(float deg)
{
    return (deg*3.141592f)/180.0f;
}

//Função para limite da rotação de câmera
float clamp(float value, float low, float high)
{
    const float t = value < low ? low : value;
    return t > high ? high : t;
}

//Função para movimentação de câmera de acordo com o movimento do mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouseDeltaX = mousePosX - xpos;
    mouseDeltaY = mousePosY - ypos;
    mousePosX = xpos;
    mousePosY = ypos;
}

//Função para comandos do teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //ESC: Fecha programa
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
    //QE: movimento da posição Y
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
        else
            rot = false;

    }

    //Espaço: Pular
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

//Função de construção do objeto checkpoint
SpawnPoint* createSpawnPoint(SceneManager& sceneManager, Mesh3D* crystalMesh, Material* crystalMaterial)
{
    SpawnPoint* SP = new SpawnPoint("spawnPoint");
    TriggerVolume* TG = new TriggerVolume("spTG");
    CollisionShape* CS = new CollisionShape("spCS");

    NodeMesh3D* crystal = new NodeMesh3D("crystal", crystalMesh, crystalMaterial);
    PointLight* light = new PointLight("lt");
    light->setAttenuationRadius(16.0f);
    light->setColor(glm::vec4(0.121f, 0.121f, 0.871f, 1.0f));
    light->setIntensity(0.0f);
    crystal->addChild(light);
    light->translate(glm::vec3(0.0f,1.7f,0.0f));

    CS->setCollisionType(COLLISION_SPHERE);
    CS->setRadius(6.0f);

    TG->setOnCollisionReceiver(SP);
    TG->addChild(CS);

    SP->addChild(TG);
    SP->addChild(crystal);
    SP->setCrystalNode(crystal);
    SP->setLightNode(light);
    SP->addChild(sceneManager.loadSceneFromFile("../DreamGliderAssets/Scenes/SpawnPoint.scn"));
    SP->scale(glm::vec3(2.0f));
    return SP;
}

//Função de construção do objeto Gate e os pedestais relacionados
PhysicsBody* createGate(SceneManager& sceneManager, std::vector<PhysicsBody*>& pedestals)
{
    PhysicsBody* gatePB = static_cast<PhysicsBody*>(sceneManager.loadSceneFromFile("../DreamGliderAssets/Scenes/Gate.scn"));
    Mesh3D* portalMesh = new Mesh3D("../DreamGliderAssets/Meshes/Islands/GateIsland/Gate/Portal.obj");
    Material* portalMat = new Material(glm::vec4(1.0f));
    NodeMesh3D* portalNM = new NodeMesh3D("portalnm", portalMesh, portalMat);

    gatePB->addChild(portalNM);
    portalNM->visible = false;

    PhysicsBody* pedestal01 = static_cast<PhysicsBody*>(sceneManager.loadSceneFromFile("../DreamGliderAssets/Scenes/Pedestal.scn"));
    PhysicsBody* pedestal02 = static_cast<PhysicsBody*>(sceneManager.loadSceneFromFile("../DreamGliderAssets/Scenes/Pedestal.scn"));
    PhysicsBody* pedestal03 = static_cast<PhysicsBody*>(sceneManager.loadSceneFromFile("../DreamGliderAssets/Scenes/Pedestal.scn"));
    PhysicsBody* pedestal04 = static_cast<PhysicsBody*>(sceneManager.loadSceneFromFile("../DreamGliderAssets/Scenes/Pedestal.scn"));
    PhysicsBody* pedestal05 = static_cast<PhysicsBody*>(sceneManager.loadSceneFromFile("../DreamGliderAssets/Scenes/Pedestal.scn"));
    PhysicsBody* pedestal06 = static_cast<PhysicsBody*>(sceneManager.loadSceneFromFile("../DreamGliderAssets/Scenes/Pedestal.scn"));

    pedestals.push_back(pedestal01);
    pedestals.push_back(pedestal02);
    pedestals.push_back(pedestal03);
    pedestals.push_back(pedestal04);
    pedestals.push_back(pedestal05);
    pedestals.push_back(pedestal06);

    pedestal01->translate(glm::vec3(-3.5f, 0.0f, 3.0f));
    pedestal02->translate(glm::vec3(-3.5f, 0.0f, 5.0f));
    pedestal03->translate(glm::vec3(-3.5f, 0.0f, 7.0f));
    pedestal04->translate(glm::vec3(3.5f, 0.0f, 3.0f));
    pedestal05->translate(glm::vec3(3.5f, 0.0f, 5.0f));
    pedestal06->translate(glm::vec3(3.5f, 0.0f, 7.0f));

    gatePB->addChild(pedestal01);
    gatePB->addChild(pedestal02);
    gatePB->addChild(pedestal03);
    gatePB->addChild(pedestal04);
    gatePB->addChild(pedestal05);
    gatePB->addChild(pedestal06);

    return gatePB;
}

//Função de carregamendo do oceano com seus respectivos shaders/texturas
NodeMesh3D* loadOcean()
{
    Mesh3D* oceanMesh = new Mesh3D("../DreamGliderAssets/Meshes/Ocean/Ocean.obj");
    Texture* oceanNormal = new Texture("../DreamGliderAssets/Materials/Water/Ocean_normal.png");
    Texture* oceanFoam = new Texture("../DreamGliderAssets/Materials/Water/Ocean_albedo.png");
    Material* oceanMaterial = new Material(glm::vec4(0.01f, 0.02f, 0.04f, 0.5f));
    oceanMaterial->setAlbedoTexture(oceanFoam);
    oceanMaterial->setNormalTexture(oceanNormal);
    oceanMaterial->setUVTiling(glm::vec2(10.0f,10.0f));
    oceanMaterial->setShaderType(SHADER_OCEAN);
    oceanMaterial->setRoughness(0.05f);
    NodeMesh3D* oceanNodeMesh = new NodeMesh3D("ocean", oceanMesh, oceanMaterial);
    oceanNodeMesh->translate(glm::vec3(0.0f,-300.0f,0.0f));
    oceanNodeMesh->scale(glm::vec3(2.0f,1.0f,2.0f));
    oceanNodeMesh->setCastsShadows(false);
    return oceanNodeMesh;
}

//Função de construção do túnel de vento
WindTube* createWindTube(Mesh3D* windTubeMesh, Texture* windTubeTexture, float radius, float windSpeed)
{
    Material* windTubeMat = new Material(windTubeTexture);
    windTubeMat->setShaderType(SHADER_PBR_SCROLL);
    windTubeMat->setUVScrolling(glm::vec2(0.0f, -windSpeed / 80.0f));
    windTubeMat->setUVTiling(glm::vec2(3.1415f * radius * 0.25, 1.0f * radius * 0.25));
    windTubeMat->setFaceCulling(false);

    WindTube* windTube = new WindTube("windTube");
    windTube->setWindPower(windSpeed);
    NodeMesh3D* windTubeNM = new NodeMesh3D("windTubeMesh", windTubeMesh, windTubeMat);
    windTubeNM->scale(glm::vec3(radius));

    CollisionShape* windTubeCol = new CollisionShape("windCol");
    TriggerVolume* windTrigger = new TriggerVolume("windTrigger");
    windTubeCol->setCollisionType(COLLISION_SPHERE);
    windTubeCol->setRadius(radius);
    windTube->addChild(windTubeNM);
    windTube->addChild(windTrigger);
    windTrigger->addChild(windTubeCol);
    windTrigger->setOnCollisionReceiver(static_cast<Node*>(windTube));
    return windTube;
}

//Função de definição dos colecionáveis
Collectible* createCollectibe(Mesh3D* mesh, Material* material, Node3D* destinationNode)
{
    Collectible* collectible = new Collectible("collec", destinationNode);
    CollisionShape* CSCol = new CollisionShape("collecCol");
    CSCol->setCollisionType(COLLISION_SPHERE);
    CSCol->setRadius(2.0f);

    PointLight* Plight = new PointLight("plight");
    Plight->setAttenuationRadius(16.0f);
    Plight->setIntensity(0.8f);
    Plight->setColor(glm::vec4(0.3f,0.7f,1.0f,1.0f));

    TriggerVolume* collTV = new TriggerVolume("collTV");
    collTV->setDoOnce(true);
    collTV->setOnCollisionReceiver(collectible);
    collTV->addChild(CSCol);

    NodeMesh3D* NM = new NodeMesh3D("collMsh", mesh, material);
    collectible->addChild(collTV);
    collectible->addChild(NM);
    collectible->addChild(Plight);
    return collectible;
}

//Função de física de objeto
void doPhysics(SceneManager* sceneManager, Node3D* camY, Camera* cam, Player* playerTest, NodeMesh3D* buny)
{
    #define PHYSICS_FRAMETIME 0.00416667
    int jFrames = 0;
    float xRot = 0.0f;
    Curves* trajeto = new Curves("trajeto");
    float deltaTime = PHYSICS_FRAMETIME;
    while (running)
    {
        float startTime = glfwGetTime();
        glm::vec4 movement = float(R - L) * camY->getGlobalBasisX() + float(BW - FW) * camY->getGlobalBasisZ();// + float(UP - DOWN) * camY->getBasisY();
        movement = glm::length(movement) < 1.0f ? movement : glm::normalize(movement);
        playerTest->addAcceleration(movement * 120.0f);
        xRot = clamp(xRot + mouseDeltaY * 0.0003,-3.141592f/2.0f, 3.141592f/2.0f);
        camY->rotateGlobalY(mouseDeltaX * 0.0003);
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
        curvature(buny,trajeto,glm::vec3(0.0f,1.4f,0.0f),glm::vec3(0.0f,3.0f,0.0f),glm::vec3(2.0f,1.7f,0.0f),glm::vec3(2.0f,1.4f,0.0f));

        sceneManager->applyPhysics(deltaTime);
        sceneManager->applyTransforms();

        deltaTime = glfwGetTime() - startTime;
        while (deltaTime < PHYSICS_FRAMETIME)
        {
            deltaTime = glfwGetTime() - startTime;
        }
    }
}

int main()
{
    //Setup do root: Node3D é uma árvore cujos ramos contém os objetos da cena
    Node3D* sceneRoot = new Node3D("scene root");

    Camera* cam = new Camera("camera", 0.2f, 3000.0f, 0.0f);
    Window* window = new Window();
    Renderer renderer(window, cam, sceneRoot);

    Environment* env = new Environment("../DreamGliderAssets/Cubemaps/Sky/sky");
    renderer.setEnvironment(env);

    SceneManager sceneManager(sceneRoot);
    Node::setSceneManager(&sceneManager);

    //Materials
    Material::initializeDefaultTextures();
    Material* defaultMat = new Material(glm::vec4(1.0f,0.7f,0.0f,0.0f));
    defaultMat->setShaderType(SHADER_PBR);
    defaultMat->setRoughness(0.4);
    defaultMat->setMetallic(1.0);

    //Inicialização de cena

    //Sol
    //Iluminação global e sombras
    glCheckError();
    DirectionalLight* sun = new DirectionalLight("SUN");
    sun->setColor(glm::vec4(1.0f,0.691f,0.5f,1.0f));
    sun->setIntensity(2.0f);
    sun->setShadowsEnabled(true);
    sun->setShadowBias(0.0007f);
    sun->setBiasSplitMultiplier(1.4);
    sun->setShadowResolution(2048);
    sun->setNumShadowSamples(12);
    glCheckError();

    Node3D* rotationTex = new Node3D("RTS");

    rotationTex->rotateGlobalX((9.4f * 3.141592f)/180.0f);
    rotationTex->rotateGlobalY((-27.3575f * 3.141592f)/180.0f);

    sceneRoot->addChild(rotationTex);
    rotationTex->addChild(sun);

    //Player
    //Define o jogador como três esferas para fim de teste de colisão
    Player* playerTest = new Player("player", cam);
    CollisionShape* playerCol = new CollisionShape("playerCol");
    playerCol->setCollisionType(COLLISION_SPHERE);
    playerCol->setRadius(0.4f);
    playerCol->translate(glm::vec3(0.0f,0.4f,0.0f));
    CollisionShape* playerCol2 = new CollisionShape("playerCol");
    playerCol2->setCollisionType(COLLISION_SPHERE);
    playerCol2->setRadius(0.4f);
    playerCol2->translate(glm::vec3(0.0f,0.85f,0.0f));
    CollisionShape* playerCol3 = new CollisionShape("playerCol");
    playerCol3->setCollisionType(COLLISION_SPHERE);
    playerCol3->setRadius(0.4f);
    playerCol3->translate(glm::vec3(0.0f,1.3f,0.0f));

    playerTest->addChild(playerCol);
    playerTest->addChild(playerCol2);
    playerTest->addChild(playerCol3);
    playerTest->translate(glm::vec3(0.0f,130.0f,0.0f));

    Node3D* camY = new Node3D("camY");
    camY->translate(glm::vec3(0.0f,1.40f,0.0f));

    camY->addChild(cam);
    playerTest->addChild(camY);

    sceneRoot->addChild(playerTest);

    //Inicialização dos objetos da cena
    //Coelho
    Mesh3D* bunnyMesh = new Mesh3D("../DreamGliderAssets/Meshes/bunny.obj");
    NodeMesh3D* buny = new NodeMesh3D( "Buny" , bunnyMesh, defaultMat);

    sceneRoot->addChild(buny);
    buny->translate(glm::vec3(50.0f,140.0f,35.0f));

    //Ilha do lago
    Node3D* pondIsland = static_cast<Node3D*>(sceneManager.loadSceneFromFile("../DreamGliderAssets/Scenes/PondIsland.scn"));
    sceneRoot->addChild(pondIsland);
    //Ilha inicial
    Node* gateIsland = sceneManager.loadSceneFromFile("../DreamGliderAssets/Scenes/GateIsland.scn");
    sceneRoot->addChild(gateIsland);
    //Ilha maçã
    Node* appleIsland = sceneManager.loadSceneFromFile("../DreamGliderAssets/Scenes/AppleIsland.scn");
    sceneRoot->addChild(appleIsland);
    //Ilha deserto
    PhysicsBody* desertIsland = static_cast<PhysicsBody*>(sceneManager.loadSceneFromFile("../DreamGliderAssets/Scenes/DesertIsland.scn"));
    desertIsland->translate(glm::vec3(450.0f, 85.0f, -620.0f));
    sceneRoot->addChild(desertIsland);
    //Ilha cidade
    PhysicsBody* cityIsland = static_cast<PhysicsBody*>(sceneManager.loadSceneFromFile("../DreamGliderAssets/Scenes/CityIsland.scn"));
    sceneRoot->addChild(cityIsland);
    cityIsland->translate(glm::vec3(-192.0f, 165.0f, 440.0f));
    //Ilha farol
    PhysicsBody* lighthouseIsland = static_cast<PhysicsBody*>(sceneManager.loadSceneFromFile("../DreamGliderAssets/Scenes/LighthouseIsland.scn"));
    sceneRoot->addChild(lighthouseIsland);
    lighthouseIsland->translate(glm::vec3(0.0f,-297.0f,0.0f));

    //SpawnPoints
    Mesh3D* crystalMesh = new Mesh3D("../DreamGliderAssets/Meshes/SpawnPoint/SpawnPointCrystal.obj");
    Material* crystalMaterial = new Material(glm::vec4(0.144f,0.02f,0.552f,1.0f));
    crystalMaterial->setShaderType(SHADER_PBR);
    crystalMaterial->setTransmission(1.0f);
    crystalMaterial->setRoughness(0.1f);
    SpawnPoint* SPP = createSpawnPoint(sceneManager, crystalMesh, crystalMaterial);
    SPP->setPosition(glm::vec3(0.0f));
    SPP->translate(glm::vec3(-2.39f,53.771f,-46.694f));
    gateIsland->addChild(SPP);

    SpawnPoint* spawnPoint02 = createSpawnPoint(sceneManager, crystalMesh, crystalMaterial);
    appleIsland->addChild(spawnPoint02);
    spawnPoint02->translate(glm::vec3(-22.525f, 44.247f, 11.584f));
    spawnPoint02->scale(glm::vec3(0.5f));

    SpawnPoint* spawnPoint03 = createSpawnPoint(sceneManager, crystalMesh, crystalMaterial);
    cityIsland->addChild(spawnPoint03);
    spawnPoint03->translate(glm::vec3(-9.0289f, 5.2358f, -14.708f));

    SpawnPoint* spawnPoint04 = createSpawnPoint(sceneManager, crystalMesh, crystalMaterial);
    desertIsland->addChild(spawnPoint04);
    spawnPoint04->translate(glm::vec3(28.949f, -1.2629f, 3.1533f));

    //Gate
    std::vector<PhysicsBody*> pedestals;
    PhysicsBody* gate = createGate(sceneManager, pedestals);
    gateIsland->addChild(gate);
    gate->translate(glm::vec3(-17.941f,53.52f,-102.04f));
    gate->rotateGlobalY(0.65051f);

    //Wind tubes
    Mesh3D windTubeMesh = Mesh3D("../DreamGliderAssets/Meshes/Misc/WindTube.obj");
    Texture windTubeTexture = Texture("../DreamGliderAssets/Materials/VFX/Wind.png", 8);

    WindTube* windTube01 = createWindTube(&windTubeMesh, &windTubeTexture, 20.0f, 40.0f);
    sceneRoot->addChild(windTube01);
    windTube01->translate(glm::vec3(-20.0f,-285.0f,66.0f));

    WindTube* windTube02 = createWindTube(&windTubeMesh, &windTubeTexture, 10.0f, 14.0f);
    sceneRoot->addChild(windTube02);
    windTube02->translate(glm::vec3(32.351f,-43.281f,-78.062f));
    windTube02->rotateLocalY(deg2rad(-22.3988f));
    windTube02->rotateLocalX(0.717330);

    WindTube* windTube03 = createWindTube(&windTubeMesh, &windTubeTexture, 16.0f, 24.0f);
    appleIsland->addChild(windTube03);
    windTube03->translate(glm::vec3(64.264f,-52.84f,37.063f));
    windTube03->rotateLocalY(deg2rad(-61.6f));
    windTube03->rotateLocalX(deg2rad(40.8));

    WindTube* windTube04 = createWindTube(&windTubeMesh, &windTubeTexture, 20.0f, 24.0f);
    desertIsland->addChild(windTube04);
    windTube04->translate(glm::vec3(37.441f,-76.862f,103.92f));


    //Collectibles
    Mesh3D* collectibleMesh01 = new Mesh3D("../DreamGliderAssets/Meshes/Artefacts/Star.obj");
    Mesh3D* collectibleMesh02 = new Mesh3D("../DreamGliderAssets/Meshes/Artefacts/Ring.obj");
    Mesh3D* collectibleMesh03 = new Mesh3D("../DreamGliderAssets/Meshes/Artefacts/Monkey.obj");
    /*Mesh3D* collectibleMesh04;
    Mesh3D* collectibleMesh05;
    Mesh3D* collectibleMesh06;*/

    Material* collectibleMaterial = new Material(glm::vec4(0.05f,0.25f,0.85f,1.0f));
    collectibleMaterial->setShaderType(SHADER_PBR);
    collectibleMaterial->setTransmission(1.0f);

    Collectible* collectible01 = createCollectibe(collectibleMesh01, collectibleMaterial, pedestals[0]);
    desertIsland->addChild(collectible01);
    collectible01->translate(glm::vec3(48.695f,7.8961f,-21.508f));


    Collectible* collectible02 = createCollectibe(collectibleMesh02, collectibleMaterial, pedestals[1]);
    sceneRoot->addChild(collectible02);
    collectible02->translate(glm::vec3(231.9787f, 31.531f, 75.006));
    collectible02->rotateLocalY(deg2rad(130.0f));


    Collectible* collectible03 = createCollectibe(collectibleMesh03, collectibleMaterial, pedestals[2]);
    appleIsland->addChild(collectible03);
    collectible03->translate(glm::vec3(3.5297f, -19.99f, 30.705));
    collectible03->rotateLocalY(deg2rad(130.0f));

    //Collectible* collectible04 = createCollectibe(collectibleMesh04, collectibleMaterial);
    //Collectible* collectible05 = createCollectibe(collectibleMesh05, collectibleMaterial);
    //Collectible* collectible06 = createCollectibe(collectibleMesh06, collectibleMaterial);

    sceneRoot->root = true;
    //Oceano
    sceneRoot->addChild(loadOcean());

    //Gerenciamento e Renderização
    glfwSetKeyCallback(window->getWindow(), key_callback);
    glfwSetCursorPosCallback(window->getWindow(), mouse_callback);
    glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window->getWindow(), 0.0, 0.0);
    glCheckError();

    renderer.setDirectionalLight(sun);

    //Movimento
    #define FRAMETIME_CAP 0.00833333
    double startTime;

    sceneManager.applyTransforms();
    playerTest->setRespawnPoint(playerTest->getGlobalPosition());

    std::thread physicsThread(doPhysics, &sceneManager, camY, cam, playerTest, buny);

    //Laço de Execução
    while (!glfwWindowShouldClose(window->getWindow()) && running)
    {
        startTime = glfwGetTime();
        double tickStart = glfwGetTime();

        renderer.render();

        mouseDeltaX = 0.0f;
        mouseDeltaY = 0.0f;

        glfwPollEvents();

        double remainder = FRAMETIME_CAP - (glfwGetTime() - tickStart);

        while(remainder > 0.0001)
        {
            remainder = FRAMETIME_CAP - (glfwGetTime() - tickStart);
        }
    }
    running = false;
    physicsThread.join();

    return 0;
}
