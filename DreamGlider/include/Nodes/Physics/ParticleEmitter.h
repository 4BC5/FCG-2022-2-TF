#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H
#include <Node3D.h>

typedef struct
{
    float lifeStart = 0.0f;
    glm::vec4 position = glm::vec4(0.0f,0.0f,0.0f,1.0f);
} particleInstance;

class ParticleEmitter : public Node3D
{
    public:
        ParticleEmitter(std::string name);
        virtual ~ParticleEmitter();

        void setParticleCount(unsigned int pCount);
        void setTransparent(bool transparent);

    protected:

    private:
        unsigned int particleCount = 100;
        std::vector<particleInstance> particles;
        bool transparent = false;
        bool lit = false;
};

#endif // PARTICLEEMITTER_H
