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

    protected:

    private:
        Camera* camera;
        bool onFloor = false;
        float fDamping = 18.0f;
        float aDamping = 0.1f;
        float jumpImpulse = 8.0f;
        bool willJump = false;
        bool flightActivated = false;
};

#endif // PLAYER_H
