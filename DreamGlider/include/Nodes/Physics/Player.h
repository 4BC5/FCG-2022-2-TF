#ifndef PLAYER_H
#define PLAYER_H
#include <PhysicsBody.h>

//Classe que representa o jogador
class Player : public PhysicsBody
{
    public:
        Player(std::string name, Camera* cam);
        virtual ~Player();

        void doMovement(float deltaTime);//Override de doMovement do PhysicsBody
        void addAcceleration(glm::vec4 accel);//Adicionar acelera��o no jogador
        void jump();//Pulo do jogador

        void onReceiveMessage(std::string message, std::string argumentType, void* valuePtr);//Override de onReceiveMessage
        void setRespawnPoint(glm::vec4 respawnPoint){this->respawnPoint = respawnPoint;}//Define o ponto de respawn do jogador

    protected:

    private:
        Camera* camera;//C�mera que o jogador usa (s� para controlar o FOV)
        bool onFloor = false;//Se o jogador est� no ch�o
        float fDamping = 18.0f;//Atrito quando o jogador est� no ch�o
        float aDamping = 0.025f;//Atrito quando o jogador est� no ar
        float jumpImpulse = 5.0f;//Impulso quando o jogador pula
        bool willJump = false;//Se o jogador deve pular este frame
        bool flightActivated = false;//Se o jogador est� voando
        float vmDec = 0.0f;//Smoothing da transi��o entre cair e voar
        glm::vec4 wind = glm::vec4(0.0f);//Vetor do vento (vem do wind tunnel)
        glm::vec4 respawnPoint = glm::vec4(0.0f);//Ponto de respawn do jogador

        void respawn();
        void deactivateFlight();
};

#endif // PLAYER_H
