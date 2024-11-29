#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "planet.h"
#include "orbit.h"

extern float earthRotationAngle;
extern Orbit earthOrbit;

void renderPlanet(){
    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();
    glTranslatef(earthOrbit.getX(), 0.0f, earthOrbit.getZ());
    glRotatef(earthRotationAngle, 0.0f, 1.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Sphere wasn't mapped how I like so rotate it -90
    gluQuadricTexture(quad, GL_TRUE);
    glPushMatrix();
    gluSphere(quad, 1.0, 50, 50); // Render sphere w/ texture
    glPopMatrix();
    gluDeleteQuadric(quad);
}