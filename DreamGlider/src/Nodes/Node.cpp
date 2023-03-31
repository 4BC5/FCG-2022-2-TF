#include "Nodes/Node.h"
#include <algorithm>
#include <matrices.h>
#include <iostream>

SceneManager* Node::sceneManager = nullptr;

Node::Node(std::string name)
{
    this->name = name;
    //ctor
}

Node::~Node()
{
    parent->removeChild(this);
}

void Node::addChild(Node* newChild)
{
    if (newChild->root)
        return;
    if (newChild->parent)
        newChild->parent->removeChild(newChild);
    children.push_back(newChild);
    newChild->parent = this;
}

Node* Node::removeChild(Node* child)
{
    std::vector<Node*>::iterator childIndex = getChildIndex(child);
    return removeChildAtIndex(childIndex);
}

Node* Node::removeChildAtIndex(std::vector<Node*>::iterator childIndex)
{
    Node* removedChild = *childIndex;
    removedChild->parent = nullptr;
    children.erase(childIndex);
    return removedChild;
}

void Node::queueFree()
{
    delete this;
}

std::vector<Node*>::iterator Node::getChildIndex(Node* child)
{
    return find(children.begin(), children.end(), child);
}

/*glm::mat4 Node::getTransform()
{
    return mop::Matrix_Identity();
}*/


Node* Node::getParent()
{
    return parent;
}

std::vector<Node*> Node::getChildren()
{
    return children;
}

void Node::onSceneSetup()
{

}

//Scene manager deve poder ser acessado por todos os nodes
void Node::setSceneManager(SceneManager* snMngr)
{
    Node::sceneManager = snMngr;
}

//Al�m de atribuir true a moved, propaga moved para filhos
void Node::setMoved()
{
    moved = true;
    unsigned int cCount = children.size();
    for (unsigned int i = 0; i < cCount; ++i)
    {
        children[i]->setMoved();
    }
}
