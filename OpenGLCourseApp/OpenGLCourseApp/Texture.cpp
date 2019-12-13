#include "Texture.h"



Texture::Texture()
    :textureID(0), width(0), height(0), bitDepth(0), fileLocation("")
{
}

Texture::Texture(const char * fileLoc)
    :textureID(0), width(0), height(0), bitDepth(0), fileLocation(fileLoc)
{
}


Texture::~Texture()
{
    this->clearTexture();
}

void Texture::loadTexture()
{
    unsigned char * texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0); //This is essentially a byte array, and is common to represent image arrays
    if (!texData)
    {
        printf("Failed to load %s\n", fileLocation);
        return;
    }
    glGenTextures(1, &textureID); //Generating a texture object and getting its id
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //When it hits the border on the s axis, it'll repeat the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Filtering when you zoom out of the image
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Filtering when you zoom into the image

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData); //We set this to RGBA instead of RGB because we're using png files and can use an alpha channel
    glGenerateMipmap(GL_TEXTURE_2D); //You can generate mipmaps manually, but we're asking opengl to generate them automatically

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texData);
}

void Texture::useTexture()
{
    //GL_TEXTURE0 is a "texture unit"
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID); //binds our texture to the above texture unit
}

void Texture::clearTexture()
{
    glDeleteTextures(1, &textureID);
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = "";
}
