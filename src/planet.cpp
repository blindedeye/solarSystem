#include "planet.h"

// Constructor initializing the planet's properties
Planet::Planet(float radius, int slices, int stacks)
    : radius(radius), slices(slices), stacks(stacks) {}

// Method to draw the planet using glutSolidSphere
void Planet::draw() {
    glPushMatrix();
    glutSolidSphere(radius, slices, stacks);
    glPopMatrix();
}
