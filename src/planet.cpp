#include "planet.h"
#include "stb_image.h"
#include <iostream>

// Constructor initializing the planet's properties and loading texture
Planet::Planet(float radius, int slices, int stacks, const std::string& texturePath)
    : radius(radius), slices(slices), stacks(stacks), textureID(0) {
    loadTexture(texturePath);
}

// Destructor to clean up texture
Planet::~Planet() {
    glDeleteTextures(1, &textureID);
}

// Method to load texture from file
void Planet::loadTexture(const std::string& texturePath) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load the texture using stb_image
    int width, height, channels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);
    if (data) {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB; // Determine format based on channels
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    }
    stbi_image_free(data); // Free the image memory
}

// Method to draw the planet using glutSolidSphere with texture applied
void Planet::draw() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Draw the sphere with texture mapping enabled
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE); // Enable texturing on the quadric
    gluSphere(quad, radius, slices, stacks);
    gluDeleteQuadric(quad);

    glDisable(GL_TEXTURE_2D);
}
