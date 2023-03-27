#include "SpawnPoint.h"
#include <PhysicsBody.h>
#include <algorithm>
#include <PointLight.h>

std::vector<SpawnPoint*> SpawnPoint::spawnPoints = {};

SpawnPoint::SpawnPoint(std::string name) : Node3D(name)
{
    spawnPoints.push_back(this);
}

SpawnPoint::~SpawnPoint()
{
    auto removePos = std::find(spawnPoints.begin(), spawnPoints.end(), this);
    spawnPoints.erase(removePos);
}

void SpawnPoint::onReceiveMessage(std::string message, std::string argumentType, void* valuePtr)
{
    if (message == "trigger" && argumentType == "PhysicsBody")
    {
        if (enabled)
            return;
        PhysicsBody* pb = static_cast<PhysicsBody*>(valuePtr);
        if (pb->name == "player")
        {
            glm::vec4 spawnPoint = pb->getGlobalPosition();
            pb->onReceiveMessage("setSpawn", "vec4", static_cast<void*>(&spawnPoint));
            enableSpawnPoint();
        }
    }
}

void SpawnPoint::enableSpawnPoint()
{
    for(unsigned int i = 0; i < spawnPoints.size(); i++)
    {
        if (spawnPoints[i] != this)
        {
            spawnPoints[i]->disableSpawnPoint();
        }
    }
    enabled = true;
    receiveTick = true;
}

void SpawnPoint::disableSpawnPoint()
{
    enabled = false;
    receiveTick = true;
}

void SpawnPoint::onTick(float deltaTime)
{
    if (crystalNode != nullptr)
    {
        crystalNode->setPosition(glm::vec3(0.0f,lerpT * 0.25f,0.0f));
        lightNode->setIntensity(lerpT * 16.0f);
        if (enabled)
        {
            lerpT = std::min(1.0f, lerpT + deltaTime);
            if (lerpT == 1.0f)
            {
                receiveTick = false;
            }
        }
        else
        {
            lerpT = std::max(0.0f, lerpT - deltaTime);
            if (lerpT == 0.0f)
            {
                receiveTick = false;
            }
        }
    }
}
