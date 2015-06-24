
#include "main.h"

Global g;

// glut display function
void displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT);
    g.knot.glRender(g.windowWidth, g.windowHeight);
    glFlush();
}


int main(int argc, char** argv) {

    g.windowWidth = 300;
    g.windowHeight = 300;

    g.knot = CelticGrid("sample-knot.txt");
    g.knot.setCellSize(g.windowWidth / g.knot.width());

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(g.windowWidth, g.windowHeight);
    glutInitWindowPosition(30, 30);
    g.glutWindow = glutCreateWindow("celtic knots");

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION | GL_MATRIX_MODE);
    glLoadIdentity();
    glOrtho(-10, g.windowWidth+10, -10, g.windowHeight+10, 0, 1);

    glutDisplayFunc(displayFunc);

    glutMainLoop();
    return 0;
}
