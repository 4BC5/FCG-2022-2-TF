#include <UI/UIElement.h>
#include <Renderer/Renderer.h>
#include <Nodes/Node.h>
#include <Renderer/Camera/Camera.h>
#include <Nodes/NodeMesh3D.h>
#include <Window/Window.h>

//#include <dejavufont.h>


UIElement::UIElement()//(Window* window, Camera* cam, Node* root, Camera* directional) : Renderer(window, cam, root, directional)
{
    //ctor
    // Inicializamos o código para renderização de texto.
   // this->TextRendering_Init(renderer);
}

UIElement::~UIElement()
{
    //dtor
}


