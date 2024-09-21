#include <GL/glew.h>

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "planet.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <iomanip>

using namespace std;

GLuint shaderProgram;
Planet earth(1.0f, 50, 50, "path_to_your_texture.jpg"); // Create a planet object with texture

// Rotation angles and zoom level
float angleX = 0.0f;
float angleY = 0.0f;
float zoom = -5.0f; // Initial zoom level (camera distance)
int lastMouseX, lastMouseY;
bool isDragging = false;

// Function to read shader source code from a file
string readShaderSource(const char* filePath) {
    ifstream shaderFile(filePath);
    stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    return shaderStream.str();
}

// Function to compile a shader
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        cerr << "Error: Shader Compilation Failed\n" << infoLog << endl;
    }

    return shader;
}

// Function to set up shaders
void setupShaders() {
    string vertexCode = readShaderSource("vertex.glsl");
    string fragmentCode = readShaderSource("fragment.glsl");

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        cerr << "Error: Shader Linking Failed\n" << infoLog << endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// OpenGL display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram);

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);

    // Set up the model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0,  // Camera position
              0.0, 0.0, 0.0,  // Look at position
              0.0, 1.0, 0.0); // Up vector

    glPushMatrix();

    // Apply zoom (camera translation)
    glTranslatef(0.0f, 0.0f, zoom);

    // Apply rotations
    glRotatef(angleX, 1.0f, 0.0f, 0.0f); // Rotate around X-axis
    glRotatef(angleY, 0.0f, 1.0f, 0.0f); // Rotate around Y-axis

    // Draw the planet
    earth.draw();

    glPopMatrix();
    glutSwapBuffers();
}

// Mouse event handlers
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        isDragging = true;
        lastMouseX = x;
        lastMouseY = y;
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        isDragging = false;
    }

    // Handle mouse wheel for zooming
    if (button == 3) { // Scroll up
        zoom += 0.5f; // Zoom out
    } else if (button == 4) { // Scroll down
        zoom -= 0.5f; // Zoom in
    }
    glutPostRedisplay();
}

// Mouse motion handler for dragging
void motion(int x, int y) {
    if (isDragging) {
        // Calculate the rotation angle based on mouse movement
        angleX += (y - lastMouseY) * 0.5f; // Adjust sensitivity as needed
        angleY += (x - lastMouseX) * 0.5f; // Adjust sensitivity as needed

        // Update last mouse positions
        lastMouseX = x;
        lastMouseY = y;

        // Redisplay the updated rotation
        glutPostRedisplay();
    }
}

// Initialization function
void init() {
    glEnable(GL_DEPTH_TEST);
    setupShaders();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Rotatable and Textured Planet");

    glewInit();
    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();

    return 0;
}