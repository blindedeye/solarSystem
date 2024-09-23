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

// GLM for matrix transformations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

GLuint shaderProgram;
Planet earth(1.0f, 50, 50); // Create a planet object with radius, slices, and stacks

// Rotation angles and zoom level
float angleX = 0.0f;
float angleY = 0.0f;
float zoom = -5.0f; // Initial zoom level (camera distance)
int lastMouseX, lastMouseY;
bool isDragging = false;

// Global variables to store the latest mouse coordinates
int currentMouseX = 0;
int currentMouseY = 0;

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
    string vertexCode = readShaderSource("src/vertex.glsl");
    string fragmentCode = readShaderSource("src/fragment.glsl");

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

// Function to set uniform values before drawing
void setUniforms(int mouseX, int mouseY) {
    glUseProgram(shaderProgram);

    // Set the transformation matrices (model, view, projection)
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

    // Define the transformation matrices
    glm::mat4 model = glm::mat4(1.0f);  // Start with an identity matrix

    // Apply rotations around the X and Y axes
    model = glm::rotate(model, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
    model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis

    // Instead of applying zoom to the model, apply it to the view matrix
    glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 5.0 + zoom); // Adjust the camera position with zoom
    glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Send the transformation matrices to the shaders
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Set light and material properties
    GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    GLint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    GLint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");

    // Set these uniforms according to your scene setup
    glUniform3f(lightPosLoc, 1.2f, 1.0f, 2.0f); // Example light position
    glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z); // Update to the new camera position
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); // White light
    glUniform3f(objectColorLoc, 0.6f, 0.7f, 0.8f); // Example object color
}

// OpenGL display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set the shader uniforms with the current mouse coordinates
    setUniforms(currentMouseX, currentMouseY);

    // Since view is now handled inside setUniforms, we no longer need
    // to explicitly set the projection and model-view here in legacy style.
    glPushMatrix();

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
    // Update current mouse position for zoom targeting
    currentMouseX = x;
    currentMouseY = y;
}

// Optional: Use passive motion to update mouse position when not dragging
// void passiveMotion(int x, int y) {
//     currentMouseX = x;
//     currentMouseY = y;
//     glutPostRedisplay();
// }

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
    glutCreateWindow("Rotatable and Zoomable Planet");

    glewInit();
    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    // glutPassiveMotionFunc(passiveMotion); // Track mouse even when not clicking
    glutMainLoop();

    return 0;
}