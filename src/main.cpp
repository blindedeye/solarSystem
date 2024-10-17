#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>
#include "planet.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // library for easier texture mapping

using namespace std;

GLuint earthTexture;
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
bool shouldWarp = false; // cursor recentering (makes more sense after you run)


void loadTexture(const char* filename) {
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  // Flip image vertically
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (!data) {
        cerr << "Failed to load texture" << endl;
        return;
    }
    glGenTextures(1, &earthTexture);
    glBindTexture(GL_TEXTURE_2D, earthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (nrChannels == 4) gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}

void updateCameraDirection() {
    // Convert degrees to radians
    float yawRad = cameraYaw * (M_PI / 180.0f);
    float pitchRad = cameraPitch * (M_PI / 180.0f);

    // Calc front vector
    cameraFrontX = cosf(yawRad) * cosf(pitchRad);
    cameraFrontY = sinf(pitchRad);
    cameraFrontZ = sinf(yawRad) * cosf(pitchRad);

    // Const movement speed
    float length = sqrt(cameraFrontX * cameraFrontX + cameraFrontY * cameraFrontY + cameraFrontZ * cameraFrontZ);
    cameraFrontX /= length;
    cameraFrontY /= length;
    cameraFrontZ /= length;
}

void setupView() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0); // Basic perspective
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ,                    
              cameraPosX + cameraFrontX, cameraPosY + cameraFrontY, cameraPosZ + cameraFrontZ,
              cameraUpX, cameraUpY, cameraUpZ);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, earthTexture);
    glPushMatrix();
    renderPlanet();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

void updateCameraPosition() {
    float cameraRightX = cameraUpY * cameraFrontZ - cameraUpZ * cameraFrontY;
    float cameraRightY = cameraUpZ * cameraFrontX - cameraUpX * cameraFrontZ;
    float cameraRightZ = cameraUpX * cameraFrontY - cameraUpY * cameraFrontX;
    float rightLength = sqrt(cameraRightX * cameraRightX + cameraRightY * cameraRightY + cameraRightZ * cameraRightZ);
    cameraRightX /= rightLength;
    cameraRightY /= rightLength;
    cameraRightZ /= rightLength;
    if (moveForward) {
        cameraPosX += cameraFrontX * cameraSpeed;
        cameraPosY += cameraFrontY * cameraSpeed;
        cameraPosZ += cameraFrontZ * cameraSpeed;
    }
    if (moveBackward) {
        cameraPosX -= cameraFrontX * cameraSpeed;
        cameraPosY -= cameraFrontY * cameraSpeed;
        cameraPosZ -= cameraFrontZ * cameraSpeed;
    }
    if (moveLeft) {
        cameraPosX -= cameraRightX * cameraSpeed;
        cameraPosZ -= cameraRightZ * cameraSpeed;
    }
    if (moveRight) {
        cameraPosX += cameraRightX * cameraSpeed;
        cameraPosZ += cameraRightZ * cameraSpeed;
    }
}

// Rotation
void mouseMotion(int x, int y) {
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

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': moveForward = true; break;
        case 's': moveBackward = true; break;
        case 'd': moveLeft = true; break;
        case 'a': moveRight = true; break;
        case 27: exit(0); break;
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': moveForward = false; break;
        case 's': moveBackward = false; break;
        case 'd': moveLeft = false; break;
        case 'a': moveRight = false; break;
        case 27: exit(0); break;
    }
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black
    loadTexture("include/images/earth.jpg");
    setupView();
    glutSetCursor(GLUT_CURSOR_NONE); // Hide cursor
}

void idle() {
    updateCameraPosition();
    setupView();
    if (shouldWarp) { // Cursor would lock up so found this to avoid the freezing
        glutWarpPointer(centerX, centerY);
        shouldWarp = false;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Textured Sphere - Earth");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(mouseMotion); // 'free look'
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutWarpPointer(centerX, centerY);
    glutMainLoop();
    return 0;
}
