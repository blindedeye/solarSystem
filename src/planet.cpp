#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "planet.h"

// Function to render the textured planet
void renderPlanet() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE); // Enable texture mapping on the quadric

    glPushMatrix();
    gluSphere(quad, 1.0, 50, 50); // Render the sphere with texture
    glPopMatrix();

    gluDeleteQuadric(quad);
}
