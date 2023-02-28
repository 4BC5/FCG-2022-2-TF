#include <iostream>

#include <Window/Window.h>
#include <SceneManager.h>
#include <Nodes/Node3D.h>
#include <Nodes/NodeMesh3D.h>
#include <Renderer/Renderer.h>
#include <Camera.h>

#include <iostream>
#include <matrices.h>

using namespace std;

int main()
{
    Window* window = new Window();
    Node3D* sceneRoot = new Node3D();
    NodeMesh3D* cube = new NodeMesh3D("C:\\Users\\victf\\Documents\\Faculdade\\FCG\\DreamGliderAssets\\Meshes\\Cube.obj");
    NodeMesh3D* cube2 = new NodeMesh3D("C:\\Users\\victf\\Documents\\Faculdade\\FCG\\DreamGliderAssets\\Meshes\\Cube.obj");
    Camera* cam = new Camera();
    sceneRoot->addChild(cube2);
    sceneRoot->addChild(cube);
    sceneRoot->addChild(cam);

    sceneRoot->root = true;
    cube->name = "Cube";
    cube2->name = "Cube2";
    cam->name = "Camera";


    cube->translate(glm::vec3(2.0f,0.0f,-8.0f));
    cube2->translate(glm::vec3(0.0f,0.0f,8.0f));
    mop::PrintMatrix(cam->getTransform());

    SceneManager sceneManager(sceneRoot);
    Renderer renderer(window, cam, sceneRoot);

    bool running = true;
    while (running)
    {
        sceneManager.applyTransforms();
        renderer.render();
        glfwPollEvents();
    }

    return 0;
}
