#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb_image.h>
#include <vector>
#include <string>
#include <iostream>
//Classe de definição de texturas
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

        int anisoLevel;
        std::string texturePath;
        GLuint textureId = 0;

};

#endif // TEXTURE_H
