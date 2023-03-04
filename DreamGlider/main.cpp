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

int main()
{
    Material* defaultMat = new Material(glm::vec4(0.5f));
    Material* wood = new Material("../DreamGliderAssets/Materials/MossyTreeBark/MossyTreeBark_albedo.png", "../DreamGliderAssets/Materials/MossyTreeBark/MossyTreeBark_normal.png");
    Material* grass = new Material("../DreamGliderAssets/Materials/Grass/Grass_albedo.png","../DreamGliderAssets/Materials/Grass/Grass_normal.png");
    Material* leaves = new Material("../DreamGliderAssets/Materials/Leaves/Leaves_albedo.png");

    wood->normalStrength = 0.8;
    grass->UVtiling = glm::vec2(60.0f);
    leaves->shaderType = SHADER_BLINN_PHONG_ALPHA_DISCARD;
    leaves->faceCulling = false;


    Window* window = new Window();

    Node3D* sceneRoot = new Node3D("scene root");
    Node3D* player = new Node3D("player");

    NodeMesh3D* tree = new NodeMesh3D("Tree" ,"../DreamGliderAssets/Meshes/Trees/Tree01.obj", wood);
    NodeMesh3D* treeLeaves = new NodeMesh3D("Leaves", "../DreamGliderAssets/Meshes/Trees/Tree01Leaves.obj", leaves);
    NodeMesh3D* tree2 = new NodeMesh3D("Tree2" ,"../DreamGliderAssets/Meshes/Trees/Tree01.obj", wood);
    NodeMesh3D* cube = new NodeMesh3D( "Cube" ,"../DreamGliderAssets/Meshes/Cube.obj", grass);
    NodeMesh3D* screen = new NodeMesh3D( "Screen" ,"../DreamGliderAssets/Meshes/Screen.obj", wood);

    Camera* sun = new Camera("SUN", 0.1, 200.0, 0.0);;

    sceneRoot->addChild(tree);
    sceneRoot->addChild(tree2);
    sceneRoot->addChild(player);

    tree->addChild(treeLeaves);

    player->addChild(cam);
    player->addChild(sun);

    sceneRoot->addChild(cube);
    sceneRoot->addChild(screen);

    screen->translate(glm::vec3(0.0f,1.0f,-8.0f));
    //cam->addChild(sun);
    sun->translate(glm::vec3(0.0f,80.0f,0.0f));
    sun->rotateGlobalX(-3.141592f/2.0f);
    screen->rotateGlobalX(-3.141592f/4.0f);
    cube->scale(glm::vec3(30.0f,1.0f,30.0f));
    cube->translate(glm::vec3(0.0f,-1.0f,0.0f));
    tree->translate(glm::vec3(0.0f,0.0f,0.0f));
    tree->translate(glm::vec3(15.0f,0.0f,0.0f));
    player->translate(glm::vec3(0.0f,1.70f,2.0f));

    //std::cout << "SUN TRS: \n";
    //mop::PrintMatrix(sun->getTransform());

    sceneRoot->root = true;

    SceneManager sceneManager(sceneRoot);
    Renderer renderer(window, cam, sceneRoot, sun);

    glfwSetKeyCallback(window->getWindow(), key_callback);



    /*cube->vertices = {glm::vec4(-0.5f, -0.5f, -1.0f, 1.0f),
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

    GLint modelUniform         = glGetUniformLocation(g_GpuProgramID, "model"); // Variável da matriz "model"
    GLint viewUniform          = glGetUniformLocation(g_GpuProgramID, "view"); // Variável da matriz "view" em shader_vertex.glsl
    GLint projectionUniform    = glGetUniformLocation(g_GpuProgramID, "projection");
    GLint lightSpaceUniform    = glGetUniformLocation(g_GpuProgramID, "lightSpaceMatrix");

    glm::mat4 projection = cam->getProjectionMatrix(0.0f);
    glm::mat4 view = cam->getCameraMatrix();
    glm::mat4 globalTransform = cube->getTransform();

    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(globalTransform));
    glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));


    while (running)
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, cube->triangles.size(), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window->getWindow());
        glCheckError();
    }*/
    double startTime = glfwGetTime();
    while (running)
    {
        double deltaTime = glfwGetTime() - startTime;
        glm::vec4 movement = (float)(R - L) * cam->getGlobalBasisX() + (float)(BW - FW) * cam->getGlobalBasisZ();
        cam->rotateGlobalY(rotation * (float)deltaTime * 10000.0f);
        if (globalTrs)
        {
            player->globalTranslate(glm::vec3(movement.x, movement.y, movement.z) * (float)deltaTime * 40000.0f);
        }
        else
        {
            player->translate(glm::vec3(movement.x, movement.y, movement.z) * (float)deltaTime * 40000.0f);
        }
        sceneManager.applyTransforms();
        renderer.render();
        glfwPollEvents();
        //mop::PrintMatrix(cam->getTransform());
        startTime = glfwGetTime();
        glCheckError();
    }
    mop::PrintMatrix(sun->getTransform());

    return 0;
}
