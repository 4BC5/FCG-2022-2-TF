#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include <glad/glad.h>
#include <string>

class Environment
{
    public:
        Environment(std::string cubemapPath);
        virtual ~Environment();

        void setCubemapTextures();

    protected:

    private:
        std::string cubemapPath;
        GLuint cubemapId;
};

#endif // ENVIRONMENT_H
