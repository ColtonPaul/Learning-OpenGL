#pragma once

#include <GL\glew.h>
#include "stb_image.h"

class Texture
{
public:
    Texture();
    Texture(const char* fileLoc);
    ~Texture();

    void loadTexture();
    void useTexture();
    void clearTexture();

private:
    GLuint textureID;
    int width, height, bitDepth;

    const char* fileLocation;
};

