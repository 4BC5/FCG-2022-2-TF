#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H
#include <Node3D.h>

class Collectible : public Node3D
{
    public:
        Collectible(const std::string& name, Node3D* destination);
        virtual ~Collectible();

        void onReceiveMessage(std::string message, std::string argumentType, void* valuePtr);
        void onTick(float deltaTime);

    protected:
        static int collected;
    private:
        Node3D* destination = nullptr;
        glm::vec4 destPos = glm::vec4(0.0f);
        float travelTime = 0.0f;

        void goToPedestal();

};

#endif // COLLECTIBLE_H
