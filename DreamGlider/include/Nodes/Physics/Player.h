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
        void addAcceleration(glm::vec4 accel);//Adicionar aceleração no jogador
        void jump();//Pulo do jogador

        void onReceiveMessage(std::string message, std::string argumentType, void* valuePtr);//Override de onReceiveMessage
        void setRespawnPoint(glm::vec4 respawnPoint){this->respawnPoint = respawnPoint;}//Define o ponto de respawn do jogador

    protected:

    private:
        Camera* camera;//Câmera que o jogador usa (só para controlar o FOV)
        bool onFloor = false;//Se o jogador está no chão
        float fDamping = 18.0f;//Atrito quando o jogador está no chão
        float aDamping = 0.025f;//Atrito quando o jogador está no ar
        float jumpImpulse = 5.0f;//Impulso quando o jogador pula
        bool willJump = false;//Se o jogador deve pular este frame
        bool flightActivated = false;//Se o jogador está voando
        float vmDec = 0.0f;//Smoothing da transição entre cair e voar
        glm::vec4 wind = glm::vec4(0.0f);//Vetor do vento (vem do wind tunnel)
        glm::vec4 respawnPoint = glm::vec4(0.0f);//Ponto de respawn do jogador

        void respawn();
        void deactivateFlight();
};

#endif // PLAYER_H
