
#include "main.h"

Global g;

// glut display function
void displayFunc() {
    glFlush();
}


int main(int argc, char** argv) {
    g.windowWidth = 600;
    g.windowHeight = 600;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(g.windowWidth, g.windowHeight);
    glutInitWindowPosition(30, 30);
    g.glutWindow = glutCreateWindow("celtic knots");

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION | GL_MATRIX_MODE);
    glLoadIdentity();
    glOrtho(0, g.windowWidth, 0, g.windowHeight, 0, 1);

    glutDisplayFunc(displayFunc);

    glutMainLoop();
    return 0;
}
