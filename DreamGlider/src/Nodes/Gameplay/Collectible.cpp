#include "Collectible.h"
#include <PhysicsBody.h>

int Collectible::collected = 0;

Collectible::Collectible(const std::string& name, Node3D* destination) : Node3D(name)
{
    this->destination = destination;
}

Collectible::~Collectible()
{
    //dtor
}

void Collectible::onReceiveMessage(std::string message, std::string argumentType, void* valuePtr)
{
    if (message == "trigger" && argumentType == "PhysicsBody")
    {
        PhysicsBody* physBody = static_cast<PhysicsBody*>(valuePtr);
        if (physBody->name == "player")
            goToPedestal();
    }
}

void Collectible::goToPedestal()
{
    destPos = destination->getGlobalPosition() + glm::vec4(0.0f,3.5f,0.0f,0.0f);
    receiveTick = true;
}

void Collectible::onTick(float deltaTime)
{
    travelTime = std::min(1.0f, travelTime + deltaTime * 0.25f);
    glm::vec4 destVec = (destPos - getGlobalPosition());
    float distSq = glm::dot(destVec, destVec) / 10.0f;
    globalTranslate(((glm::vec3(glm::normalize(destVec)) + (1.0f - travelTime) * glm::vec3(0.0f,2.0f,0.0f)) * deltaTime * std::max(std::min(distSq, 32.0f),0.5f)) * travelTime);
    if(distSq < 0.0001f)
    {
        receiveTick = false;
        Collectible::collected += 1;
    }
}
