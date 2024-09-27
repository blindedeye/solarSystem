#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>
#include "planet.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Texture ID
GLuint earthTexture;

// Camera variables
float cameraPosX = 0.0f, cameraPosY = 0.0f, cameraPosZ = 5.0f; // Camera position
float cameraFrontX = 0.0f, cameraFrontY = 0.0f, cameraFrontZ = -1.0f; // Camera direction
float cameraUpX = 0.0f, cameraUpY = 1.0f, cameraUpZ = 0.0f; // Camera up vector
float cameraYaw = -90.0f; // Initial yaw angle (facing forward)
float cameraPitch = 0.0f; // Initial pitch angle
float cameraSpeed = 0.1f; // Base speed of camera movement
float mouseSensitivity = 0.1f; // Mouse sensitivity for looking around

// Center coordinates for the window
const int centerX = 400;
const int centerY = 300;

// Movement states for smooth movement
bool moveForward = false, moveBackward = false, moveLeft = false, moveRight = false;

// Flags to control cursor re-centering
bool shouldWarp = false;

// Function to load the texture using stb_image
void loadTexture(const char* filename) {
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  // Flip the image vertically
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture" << std::endl;
        return;
    }

    glGenTextures(1, &earthTexture);
    glBindTexture(GL_TEXTURE_2D, earthTexture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture data and generate mipmaps
    if (nrChannels == 4) {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    }

    stbi_image_free(data);
}

// Update the camera direction based on yaw and pitch
void updateCameraDirection() {
    // Convert degrees to radians by multiplying by (PI / 180)
    float yawRad = cameraYaw * (M_PI / 180.0f);
    float pitchRad = cameraPitch * (M_PI / 180.0f);

    // Calculate the front vector based on the yaw and pitch angles
    cameraFrontX = cosf(yawRad) * cosf(pitchRad);
    cameraFrontY = sinf(pitchRad);
    cameraFrontZ = sinf(yawRad) * cosf(pitchRad);

    // Normalize the direction vector to maintain consistent movement speed
    float length = sqrt(cameraFrontX * cameraFrontX + cameraFrontY * cameraFrontY + cameraFrontZ * cameraFrontZ);
    cameraFrontX /= length;
    cameraFrontY /= length;
    cameraFrontZ /= length;
}

// Set up basic projection and model-view matrices
void setupView() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0); // Basic perspective setup

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ,                    // Camera position
              cameraPosX + cameraFrontX, cameraPosY + cameraFrontY, cameraPosZ + cameraFrontZ, // Look-at point
              cameraUpX, cameraUpY, cameraUpZ);                     // Up vector
}

// Display function to render the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, earthTexture);

    glPushMatrix();

    // Render the planet
    renderPlanet();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

// Update the camera position based on movement states
void updateCameraPosition() {
    float cameraRightX = cameraUpY * cameraFrontZ - cameraUpZ * cameraFrontY;
    float cameraRightY = cameraUpZ * cameraFrontX - cameraUpX * cameraFrontZ;
    float cameraRightZ = cameraUpX * cameraFrontY - cameraUpY * cameraFrontX;

    // Normalize the right vector
    float rightLength = sqrt(cameraRightX * cameraRightX + cameraRightY * cameraRightY + cameraRightZ * cameraRightZ);
    cameraRightX /= rightLength;
    cameraRightY /= rightLength;
    cameraRightZ /= rightLength;

    // Move camera based on direction
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

// Mouse motion handler for rotating the camera
void mouseMotion(int x, int y) {
    // Calculate the offset from the center
    float xOffset = x - centerX;
    float yOffset = centerY - y; // Reversed since y-coordinates range bottom to top

    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    cameraYaw += xOffset;
    cameraPitch += yOffset;

    // Constrain the pitch angle to avoid screen flipping
    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;

    updateCameraDirection(); // Update the camera direction with new yaw and pitch

    // Set flag to re-center cursor on the next idle call
    shouldWarp = true;
}

// Keyboard handler for movement states
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': moveForward = true; break;
        case 's': moveBackward = true; break;
        case 'd': moveLeft = true; break;
        case 'a': moveRight = true; break;
    }
}

// Keyboard release handler to stop movement
void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': moveForward = false; break;
        case 's': moveBackward = false; break;
        case 'd': moveLeft = false; break;
        case 'a': moveRight = false; break;
    }
}

// Initialization function
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set clear color to black
    loadTexture("include/images/earth.jpg"); // Ensure earth.jpg is in the correct directory
    setupView(); // Set up the camera and perspective

    // Hide the mouse cursor
    glutSetCursor(GLUT_CURSOR_NONE);
}

// Idle function to update camera position continuously
void idle() {
    updateCameraPosition();
    setupView();

    // Re-center cursor only if needed to avoid freezing
    if (shouldWarp) {
        glutWarpPointer(centerX, centerY);
        shouldWarp = false;
    }

    glutPostRedisplay();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Textured Sphere - Earth");

    init();

    glutDisplayFunc(display);
    glutIdleFunc(idle);                // Continuously update position
    glutPassiveMotionFunc(mouseMotion); // Use passive motion for free look
    glutKeyboardFunc(keyboard);        // Register keyboard callback for camera movement
    glutKeyboardUpFunc(keyboardUp);    // Register keyboard up callback for stopping movement

    // Center the cursor initially
    glutWarpPointer(centerX, centerY);

    glutMainLoop();
    return 0;
}
