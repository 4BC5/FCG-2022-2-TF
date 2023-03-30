#ifndef WINDTUBE_H
#define WINDTUBE_H
#include <Node3D.h>
#include <PhysicsBody.h>

//Classe WindTube implementa o t�nel de vento, que constr�i uma for�a de gravidade empurrando o jogador para uma dire��o espec�fica
class WindTube : public Node3D
{
    public:
        WindTube(const std::string& name);
        virtual ~WindTube();

        void onCollision(PhysicsBody* physBody);
        void onReceiveMessage(std::string message, std::string argumentType, void* valuePtr);

        void setWindPower(float windPower){this->windPower = windPower;}

    protected:

    private:
        float windPower = 18.0f;
};

#endif // WINDTUBE_H
