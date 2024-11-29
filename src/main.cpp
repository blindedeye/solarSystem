#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>
#include "planet.h"
#include "orbit.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // library for easier texture mapping

using namespace std;

float earthRotationAngle = 0.0f;
GLuint earthTexture, sunTexture, jupiterTexture,
       marsTexture, mercuryTexture, moonTexture,
       neptuneTexture, saturnTexture, uranusTexture,
       venusTexture, uranusRingTexture, saturnRingTexture;

// Camera implementation from yt video
float cameraPosX = 0.0f, cameraPosY = 0.0f, cameraPosZ = 5.0f;
float cameraFrontX = 0.0f, cameraFrontY = 0.0f, cameraFrontZ = -1.0f;
float cameraUpX = 0.0f, cameraUpY = 1.0f, cameraUpZ = 0.0f;
float cameraYaw = -90.0f;
float cameraPitch = 0.0f;
float cameraSpeed = 0.1f;
float mouseSensitivity = 0.1f; // for looking around
const int centerX = 400;
const int centerY = 300;
bool moveForward = false, moveBackward = false, moveLeft = false, moveRight = false;
bool shouldWarp = false; // cursor recentering

Orbit earthOrbit(10.0f, 4.0f);
Orbit moonOrbit(1.5f, 0.5f); // Moon orbiting around Earth, with smaller radius and slower speed
Orbit marsOrbit(15.0f, 2.5f);
Orbit mercuryOrbit(7.0f, 8.0f);
Orbit venusOrbit(9.0f, 3.0f);
Orbit jupiterOrbit(20.0f, 1.5f);
Orbit saturnOrbit(25.0f, 1.2f);
Orbit uranusOrbit(30.0f, 1.0f);
Orbit neptuneOrbit(35.0f, 0.8f);

void loadTexture(const char* filename, GLuint &textureID){
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (!data){
        cerr << "Failed to load texture: " << filename << endl;
        return;
    }
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (nrChannels == 4)
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}

void updateCameraDirection(){
    // degrees to radians
    float yawRad = cameraYaw * (M_PI / 180.0f);
    float pitchRad = cameraPitch * (M_PI / 180.0f);

    // front vector
    cameraFrontX = cosf(yawRad) * cosf(pitchRad);
    cameraFrontY = sinf(pitchRad);
    cameraFrontZ = sinf(yawRad) * cosf(pitchRad);

    // movement speed
    float length = sqrt(cameraFrontX * cameraFrontX + cameraFrontY * cameraFrontY + cameraFrontZ * cameraFrontZ);
    cameraFrontX /= length;
    cameraFrontY /= length;
    cameraFrontZ /= length;
}

void setupView(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ,                    
              cameraPosX + cameraFrontX, cameraPosY + cameraFrontY, cameraPosZ + cameraFrontZ,
              cameraUpX, cameraUpY, cameraUpZ);
}

void renderSun(){
    glPushMatrix();
    glDisable(GL_LIGHTING); // Disable lighting for the sun to keep it fully lit
    glTranslatef(0.0f, 0.0f, 0.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, sunTexture);
    GLUquadric* sun = gluNewQuadric();
    gluQuadricTexture(sun, GL_TRUE);
    gluSphere(sun, 2.0f, 50, 50);
    gluDeleteQuadric(sun);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING); // Re-enable lighting for the rest of the scene
    glPopMatrix();
}

void renderPlanet(Orbit &orbit, GLuint &texture, float radius){
    glPushMatrix();
    glTranslatef(orbit.getX(), 0.0f, orbit.getZ());
    GLUquadric* quad = gluNewQuadric();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    gluQuadricTexture(quad, GL_TRUE);
    gluSphere(quad, radius, 50, 50);
    gluDeleteQuadric(quad);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat lightPosition[] ={ 0.0f, 0.0f, 0.0f, 1.0f }; // same as sun pos
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    renderSun();
    renderPlanet(earthOrbit, earthTexture, 1.0f);
    glPushMatrix();
    glTranslatef(earthOrbit.getX(), 0.0f, earthOrbit.getZ());
    renderPlanet(moonOrbit, moonTexture, 0.3f);
    glPopMatrix();
    renderPlanet(marsOrbit, marsTexture, 0.8f);
    renderPlanet(mercuryOrbit, mercuryTexture, 0.5f);
    renderPlanet(venusOrbit, venusTexture, 0.9f);
    renderPlanet(jupiterOrbit, jupiterTexture, 1.5f);
    renderPlanet(saturnOrbit, saturnTexture, 1.3f);
    renderPlanet(uranusOrbit, uranusTexture, 1.2f);
    renderPlanet(neptuneOrbit, neptuneTexture, 1.1f);

    glutSwapBuffers();
}

void updateCameraPosition(){
    float cameraRightX = cameraUpY * cameraFrontZ - cameraUpZ * cameraFrontY;
    float cameraRightY = cameraUpZ * cameraFrontX - cameraUpX * cameraFrontZ;
    float cameraRightZ = cameraUpX * cameraFrontY - cameraUpY * cameraFrontX;
    float rightLength = sqrt(cameraRightX * cameraRightX + cameraRightY * cameraRightY + cameraRightZ * cameraRightZ);
    cameraRightX /= rightLength;
    cameraRightY /= rightLength;
    cameraRightZ /= rightLength;
    if (moveForward){
        cameraPosX += cameraFrontX * cameraSpeed;
        cameraPosY += cameraFrontY * cameraSpeed;
        cameraPosZ += cameraFrontZ * cameraSpeed;
    }
    if (moveBackward){
        cameraPosX -= cameraFrontX * cameraSpeed;
        cameraPosY -= cameraFrontY * cameraSpeed;
        cameraPosZ -= cameraFrontZ * cameraSpeed;
    }
    if (moveLeft){
        cameraPosX -= cameraRightX * cameraSpeed;
        cameraPosZ -= cameraRightZ * cameraSpeed;
    }
    if (moveRight){
        cameraPosX += cameraRightX * cameraSpeed;
        cameraPosZ += cameraRightZ * cameraSpeed;
    }
}

void mouseMotion(int x, int y){
    float xOffset = x - centerX;
    float yOffset = centerY - y;
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;
    cameraYaw += xOffset;
    cameraPitch += yOffset;

    // Avoid screen flips
    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;

    updateCameraDirection();
    shouldWarp = true;
}

void keyboard(unsigned char key, int x, int y){
    switch (key){
        case 'w': moveForward = true; break;
        case 's': moveBackward = true; break;
        case 'd': moveLeft = true; break;
        case 'a': moveRight = true; break;
        case 27: exit(0); break;
    }
}

void keyboardUp(unsigned char key, int x, int y){
    switch (key){
        case 'w': moveForward = false; break;
        case 's': moveBackward = false; break;
        case 'd': moveLeft = false; break;
        case 'a': moveRight = false; break;
        case 27: exit(0); break;
    }
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            cameraYaw -= 2.0f;
            updateCameraDirection();
            break;
        case GLUT_KEY_RIGHT:
            cameraYaw += 2.0f;
            updateCameraDirection();
            break;
        case GLUT_KEY_UP:
            cameraPitch += 2.0f;
            if (cameraPitch > 89.0f) cameraPitch = 89.0f;
            updateCameraDirection();
            break;
        case GLUT_KEY_DOWN:
            cameraPitch -= 2.0f;
            if (cameraPitch < -89.0f) cameraPitch = -89.0f;
            updateCameraDirection();
            break;
    }
}

void init(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Sun light properties
    GLfloat lightAmbient[] ={ 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] ={ 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightSpecular[] ={ 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPosition[] ={ 26.0f, 0.0f, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black background: future- get spacey type background

    loadTexture("include/images/earth.jpg", earthTexture);
    loadTexture("include/images/sun.jpg", sunTexture);
    loadTexture("include/images/jupiter.jpg", jupiterTexture);
    loadTexture("include/images/mars.jpg", marsTexture);
    loadTexture("include/images/mercury.jpg", mercuryTexture);
    loadTexture("include/images/moon.jpg", moonTexture);
    loadTexture("include/images/neptune.jpg", neptuneTexture);
    loadTexture("include/images/uranus.jpg", uranusTexture);
    //loadTexture("include/images/uranus_ring.jpg", uranusRingTexture);
    loadTexture("include/images/venus.jpg", venusTexture);
    loadTexture("include/images/saturn.jpg", saturnTexture);
    //loadTexture("include/images/saturn_ring.jpg", saturnRingTexture);

    setupView();
    glutSetCursor(GLUT_CURSOR_NONE); // Hide cursor
}

void idle(){
    updateCameraPosition();

    earthOrbit.updateOrbit(0.1f);
    moonOrbit.updateOrbit(0.3f); // Faster orbit around Earth
    marsOrbit.updateOrbit(0.1f);
    mercuryOrbit.updateOrbit(0.1f);
    venusOrbit.updateOrbit(0.1f);
    jupiterOrbit.updateOrbit(0.1f);
    saturnOrbit.updateOrbit(0.1f);
    uranusOrbit.updateOrbit(0.1f);
    neptuneOrbit.updateOrbit(0.1f);

    earthRotationAngle += 0.05f; // Earth rotation speed
    if (earthRotationAngle >= 360.0f){
        earthRotationAngle -= 360.0f;
    }

    setupView();
    if (shouldWarp){ // Cursor would lock up so found this to avoid the freezing
        glutWarpPointer(centerX, centerY);
        shouldWarp = false;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Solar System Sim");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(mouseMotion); // 'free look'
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeys); // arrow keys
    glutWarpPointer(centerX, centerY);
    glutMainLoop();
    return 0;
}