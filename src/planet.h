#ifndef PLANET_H
#define PLANET_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>

class Planet {
public:
    Planet(float radius, int slices, int stacks, const std::string& texturePath); // Constructor
    ~Planet();                           // Destructor
    void draw();                         // Method to draw the planet

private:
    float radius;        // Radius of the sphere
    int slices;          // Number of slices (vertical divisions)
    int stacks;          // Number of stacks (horizontal divisions)
    GLuint textureID;    // Texture ID for the planet

    void loadTexture(const std::string& texturePath); // Method to load texture
};

#endif // PLANET_H
