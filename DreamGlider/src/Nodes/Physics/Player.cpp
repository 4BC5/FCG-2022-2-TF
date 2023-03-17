#include "Player.h"

Player::Player(std::string name, Camera* cam) : PhysicsBody(name, PHYS_BODY_KINEMATIC)
{
    camera = cam;
    setAABB(AABB(6.0f, 6.0f, 6.0f));
}

Player::~Player()
{
    //dtor
}

void Player::deactivateFlight()
{
    flightActivated = false;
    vmDec = 0.0f;
}

void Player::doMovement(float deltaTime)
{
    using namespace glm;
    updateSpeed();
    vec4 globalPosition = getGlobalPosition();

    onFloor = false;
    float colliding = false;
    vec4 remainder = vec4(0.0f);
    vec4 velNorm = bodyVelocity / (bodySpeed + 0.00001f);
    vec4 floorNormal = vec4(0.0f,1.0f,0.0f,0.0f);
    for (unsigned int i = 0; i < children.size(); i++)
    {
        if (children[i]->type == NODE_TYPE_COLLISION_SHAPE)
        {
            CollisionShape* currentShape = static_cast<CollisionShape*>(children[i]);
            std::vector<collisionInfo> cols = currentShape->testNearbyCollisions(this);

            //std::cout << "Collided:" << cols[i].collided << "\n";

            int colRange = std::min(int(cols.size()), maxCollisionsPerFrame);
            for (int i = 0; i < colRange; i++)
            {
                collisionInfo col = cols[i];
                if (col.collided)
                {
                    colliding = true;
                    remainder = col.collisionNormal * dot(velNorm, col.collisionNormal);
                    bodyVelocity = (velNorm - remainder) * bodySpeed;
                    updateSpeed();
                    bodyVelocity += remainder - (dot(remainder, col.collisionNormal)/dot(col.collisionNormal, col.collisionNormal)) * col.collisionNormal;

                    globalPosition += col.collisionNormal * (col.penetrationDepth + col.penetrationDepth * col.penetrationDepth * col.penetrationDepth + 0.00001f);
                    float dt = dot(vec3(col.collisionNormal), vec3(0.0f,1.0f,0.0f));
                    if (dt > 0.707106f)
                    {
                        onFloor = true;
                        floorNormal += col.collisionNormal;
                    }
                }
            }
            floorNormal = normalize(floorNormal);
        }
    }

    vec4 modGrav = gravity;
    float modDamping = aDamping;

    if (colliding)
    {
        if (onFloor)
        {
            deactivateFlight();

            vec4 projectedAcceleration = acceleration - (dot(acceleration, floorNormal)/dot(floorNormal, floorNormal)) * floorNormal;
            projectedAcceleration.y = min(projectedAcceleration.y, 0.0f);
            bodyVelocity += projectedAcceleration * deltaTime;
            modGrav = -floorNormal * 0.2f;
            modDamping = fDamping;
        }
    }
    else
    {
        float speedM = min(max((bodySpeed - (10.0 - vmDec * 5.0))/3.0f, 0.0),1.0);
        vec4 camDir = -camera->getGlobalBasisZ();
        float dirDot = dot(camDir, vec4(0.0f,-1.0f,0.0f,0.0f));
        if (flightActivated)
        {
            modGrav = mix(1.0f, 0.3f,(1.0 - max(dirDot,0.0f)) * speedM) * gravity;
            vmDec = min(vmDec + deltaTime * 0.5f, 1.0f);

            bodyVelocity = mix(bodyVelocity, bodySpeed * camDir, (1.0 - max(0.0f, dirDot)) * speedM * (0.25f - 0.24 * (1.0f - vmDec)));
            if (bodySpeed <= 0.25f)
            {
                deactivateFlight();
            }
        }
        else if (bodyVelocity.y <= -10.0f && dirDot > 0.65)
        {
            flightActivated = true;
        }
    }

    bodyVelocity -= bodyVelocity * vec4(modDamping, 0.0f, modDamping, 0.0f) * deltaTime;
    bodyVelocity += modGrav * deltaTime;
    bodyVelocity.y = max(bodyVelocity.y, -60.0f);

    if (onFloor && willJump)
    {
        bodyVelocity += vec4(0.0f, jumpImpulse,0.0f,0.0f);
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
