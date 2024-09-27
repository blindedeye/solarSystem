#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include "planet.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Texture ID
GLuint earthTexture;

// Rotation and zoom variables
float rotationX = 0.0f, rotationY = 0.0f;
float zoom = -5.0f; // Negative value to place the camera back

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

// Set up basic projection and model-view matrices
void setupView() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0); // Basic perspective setup

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, // Camera position
              0.0, 0.0, 0.0, // Look-at point
              0.0, 1.0, 0.0); // Up vector
}

// Display function to render the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, earthTexture);

    glPushMatrix();

    // Apply zoom and basic rotations
    glTranslatef(0.0f, 0.0f, zoom);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

    // Render the planet
    renderPlanet();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

// Mouse motion handler for rotating the sphere
void mouseMotion(int x, int y) {
    static int lastX = x, lastY = y;
    rotationX += (y - lastY) * 0.5f;
    rotationY += (x - lastX) * 0.5f;
    lastX = x;
    lastY = y;
    glutPostRedisplay();
}

// Keyboard handler for zooming using Q and E keys
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'q': // Zoom in
            zoom += 0.5f;
            break;
        case 'e': // Zoom out
            zoom -= 0.5f;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

// Initialization function
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set clear color to black
    loadTexture("include/images/earth.jpg"); // Ensure earth.jpg is in the correct directory
    setupView(); // Set up the camera and perspective
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Textured Sphere - Earth");

    init();

    glutDisplayFunc(display);
    glutMotionFunc(mouseMotion);       // Register mouse motion callback
    glutKeyboardFunc(keyboard);        // Register keyboard callback for zooming

    glutMainLoop();
    return 0;
}
