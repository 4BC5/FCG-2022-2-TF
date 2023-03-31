#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb_image.h>
#include <vector>
#include <string>
#include <iostream>


//Classe simples que carrega texturas e cria objeto de textura na GPU
class Texture
{
    public:
        Texture(std::string texturePath);
        Texture(std::string texturePath,int anisoLevel);
        virtual ~Texture();

        GLuint getTextureId(){return textureId;}

    protected:

    private:
        GLuint loadTexture(std::string path);

        int anisoLevel;//Anisotropic filtering, para evitar níveis de mip muito baixos em ângulos obliquos
        std::string texturePath;
        GLuint textureId = 0;//ID da textura na GPU

};

#endif // TEXTURE_H
