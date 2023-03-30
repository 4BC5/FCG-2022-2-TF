#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H
#include <Node3D.h>

class PointLight;

class SpawnPoint : public Node3D
{
    public:
        SpawnPoint(std::string name);
        virtual ~SpawnPoint();

        static std::vector<SpawnPoint*> spawnPoints;

        void onReceiveMessage(std::string message, std::string argumentType, void* valuePtr);
        void onTick(float deltaTime);
        void setCrystalNode(Node3D* crystalNode){this->crystalNode = crystalNode;}
        void setLightNode(PointLight* lightNode){this->lightNode = lightNode;}

    protected:
        void disableSpawnPoint();

    private:
        void enableSpawnPoint();
        bool enabled = false;
        float lerpT = 0.0f;
        Node3D* crystalNode = nullptr;
        PointLight* lightNode = nullptr;
};

#endif // SPAWNPOINT_H
