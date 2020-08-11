#include "texture.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

Texture::Texture() {
	glGenTextures(1, &ID);
}

Texture::~Texture() {
    glDeleteTextures(1, &ID); 
}

void Texture::bind(int enumType) {
	this->enumType = enumType; 
	glBindTexture(enumType, ID); 
}

void Texture::vFlip(bool setVFlip) {
    stbi_set_flip_vertically_on_load(setVFlip); // tell stb_image.h to flip loaded texture's on the y-axis.
}

void Texture::generateTexture(const char* file, int rgbType) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(enumType, 0, rgbType, width, height, 0, rgbType, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(enumType);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}