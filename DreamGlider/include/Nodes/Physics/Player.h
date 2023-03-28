#ifndef PLAYER_H
#define PLAYER_H
#include <PhysicsBody.h>


class Player : public PhysicsBody
{
    public:
        Player(std::string name, Camera* cam);
        virtual ~Player();

        void doMovement(float deltaTime);
        void addAcceleration(glm::vec4 accel);
        void jump();

        void onReceiveMessage(std::string message, std::string argumentType, void* valuePtr);
        void setRespawnPoint(glm::vec4 respawnPoint){this->respawnPoint = respawnPoint;}

    protected:

    private:
        Camera* camera;
        bool onFloor = false;
        float fDamping = 18.0f;
        float aDamping = 0.025f;
        float jumpImpulse = 5.0f;
        bool willJump = false;
        bool flightActivated = false;
        float vmDec = 0.0f;
        glm::vec4 wind = glm::vec4(0.0f);
        glm::vec4 respawnPoint = glm::vec4(0.0f);

        void respawn();
        void deactivateFlight();
};

#endif // PLAYER_H
