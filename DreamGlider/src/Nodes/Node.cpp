#include "Nodes/Node.h"
#include <algorithm>
#include <matrices.h>

Node::Node(std::string name)
{
    this->name = name;
    //ctor
}

Node::~Node()
{
    parent->removeChild(parent->getChildIndex(this));
}

void Node::addChild(Node* newChild)
{
    children.push_back(newChild);
    newChild->parent = this;
}

Node* Node::removeChild(std::vector<Node*>::iterator childIndex)
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

void Node::sceneSetup()
{

}
