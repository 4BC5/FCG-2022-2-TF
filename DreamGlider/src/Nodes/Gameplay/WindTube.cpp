#include "WindTube.h"
#include <PhysicsBody.h>

WindTube::WindTube(const std::string& name) : Node3D(name)
{
    //ctor
}

WindTube::~WindTube()
{
    //dtor
}

void WindTube::onCollision(PhysicsBody* physBody)
{
    if (physBody->name == "player")
    {

    }
}

void WindTube::onReceiveMessage(std::string message, std::string argumentType, void* valuePtr)
{
    if (message == "trigger" && argumentType == "PhysicsBody")
    {
        PhysicsBody* physBody = static_cast<PhysicsBody*>(valuePtr);
        if (physBody->name == "player")
        {
            glm::vec4 wind = getGlobalBasisY() * windPower;
            physBody->onReceiveMessage("wind", "vec4", &wind);
        }
    }
}
