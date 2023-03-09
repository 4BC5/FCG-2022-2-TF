#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb_image.h>
#include <vector>
#include <string>
#include <iostream>

class Texture
{
    public:
        Texture(std::string texturePath);
        virtual ~Texture();

        GLuint getTextureId(){return textureId;}

    protected:

    private:
        GLuint loadTexture(std::string path);

        std::string texturePath;
        GLuint textureId = 0;

};

#endif // TEXTURE_H
