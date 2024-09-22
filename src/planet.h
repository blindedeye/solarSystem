#ifndef PLANET_H
#define PLANET_H

#include <GL/glew.h>
#include <GL/glut.h>

class Planet {
public:
    Planet(float radius, int slices, int stacks); // Constructor
    void draw();                                  // Method to draw the planet

private:
    float radius;  // Radius of the sphere
    int slices;    // Number of slices (vertical divisions)
    int stacks;    // Number of stacks (horizontal divisions)
};

#endif // PLANET_H
