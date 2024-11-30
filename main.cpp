#include <GL/glut.h>
#include "src/includes/loader.hpp"
#include <vector>
#include <array>
#include <iostream>

// Global variables to store the loaded data

ply_object obj("/home/omni/Desktop/me/c_r/artemis/calibrated.ply");

// Display function to render the object
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glTranslatef(0.0f, 0.0f, -5.0f); // Move the object 5 units into the screen
	obj.render();
    glFlush();
}

void reshape(int width, int height) {
    // Prevent division by zero
    if (height == 0) height = 1;
    
    // Set the viewport to cover the new window dimensions
    glViewport(0, 0, width, height);
    
    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Set a perspective projection
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    gluPerspective(45.0, aspect, 0.1, 100.0); // Field of view, aspect ratio, near and far planes
    
    // Switch back to the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(80, 80);
    glutInitWindowSize(1000, 800);
    glutCreateWindow("PLY Object Viewer");
    glutReshapeFunc(reshape);

    // Load PLY file
    obj.load();

    // Register display callback
    glutDisplayFunc(display);

    // Enter main event loop
    glutMainLoop();

    return 0;
}
