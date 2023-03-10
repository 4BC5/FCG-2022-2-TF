#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H
#include <Node3D.h>
#include <CollisionShape.h>

enum e_bodyType {PHYS_BODY_STATIC, PHYS_BODY_KINEMATIC};

class PhysicsBody : public Node3D
{
    public:
        PhysicsBody(std::string name);
        PhysicsBody(std::string name, int type);
        virtual ~PhysicsBody();


        void setBodyType(int bodyType){this->bodyType = bodyType;}
        void setGravity();

        virtual void doMovement(float deltaTime);
        virtual void addAcceleration(glm::vec4 accel);

        int getBodyType(){return bodyType;}


    protected:
        void updateSpeed();

        int maxCollisionsPerFrame = 4;
        float bodySpeed = 0.0;
        int bodyType = PHYS_BODY_STATIC;
        glm::vec4 gravity = glm::vec4(0.0f, -9.8f, 0.0f, 0.0f);
        glm::vec4 bodyVelocity = glm::vec4(0.0);
        glm::vec4 acceleration = glm::vec4(0.0f);

    private:
};

#endif // PHYSICSBODY_H
