#include "Player.h"

Player::Player(std::string name) : PhysicsBody(name, PHYS_BODY_KINEMATIC)
{

}

Player::~Player()
{
    //dtor
}

void Player::doMovement(float deltaTime)
{
    using namespace glm;
    updateSpeed();
    vec4 globalPosition = getGlobalPosition();

    onFloor = false;
    vec4 floorNormal = vec4(0.0f,1.0f,0.0f,0.0f);
    float depenDepth = 0.0f;
    for (int i = 0; i < children.size(); i++)
    {
        if (children[i]->type == NODE_TYPE_COLLISION_SHAPE)
        {
            CollisionShape* currentShape = static_cast<CollisionShape*>(children[i]);
            std::vector<collisionInfo> cols = currentShape->testNearbyCollisions();

            //std::cout << "Collided:" << cols[i].collided << "\n";

            int colRange = std::min(int(cols.size()), maxCollisionsPerFrame);
            int numCols = 0;
            for (int i = 0; i < colRange; i++)
            {
                collisionInfo col = cols[i];
                if (col.collided)
                {
                    depenDepth += std::max(col.penetrationDepth, 0.0f);
                    bodyVelocity = reflect(bodyVelocity, col.collisionNormal) * 0.1f;
                    onFloor = (dot(vec3(col.collisionNormal), vec3(0.0f,1.0f,0.0f)) > 0.707106f);
                    floorNormal += col.collisionNormal;
                    numCols++;
                }
            }
            floorNormal /= numCols;
        }
    }

    bodyVelocity = bodyVelocity + vec4(0.0f,gravity * deltaTime,0.0f,0.0f);
    if (onFloor)
    {
        bodyVelocity -= bodyVelocity * vec4(fDamping,0.0f,fDamping,0.0f) * deltaTime;
        globalPosition += floorNormal * (depenDepth + 0.00001f);
        acceleration = acceleration - (dot(acceleration, floorNormal)/dot(floorNormal, floorNormal)) * floorNormal;
        bodyVelocity += acceleration * deltaTime;
        if (willJump)
            bodyVelocity += vec4(0.0f,1.0f,0.0f,0.0f) * jumpImpulse;
    }
    else
    {
        bodyVelocity -= bodyVelocity * vec4(aDamping,0.0f,aDamping,0.0f) * deltaTime;
    }
    willJump = false;
    acceleration = vec4(0.0f);
    globalPosition += bodyVelocity * deltaTime;
    setGlobalPosition(vec3(globalPosition));
}

void Player::addAcceleration(glm::vec4 accel)
{
    acceleration = accel;
}

void Player::jump()
{
    willJump = true;
}
