#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "planet.h"

void renderPlanet() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    glPushMatrix();
    gluSphere(quad, 1.0, 50, 50); // Render sphere w/ texture
    glPopMatrix();
    gluDeleteQuadric(quad);
}
