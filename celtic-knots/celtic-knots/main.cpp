
#include "main.h"

Global g;

// glut display function
void displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT);
    g.knot.glRender(g.windowWidth, g.windowHeight);


    glFlush();
}

/**
    glut keyboard func
    q - quit
**/
void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
    case 'q':
        glutLeaveMainLoop();
        break;
    }
}


int main(int argc, char** argv) {

    g.windowWidth = 600;
    g.windowHeight = 600;

    g.knot = CelticGrid("sample-knot.txt");
    g.knot.set_cell_size(g.windowWidth / g.knot.width());

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(g.windowWidth, g.windowHeight);
    glutInitWindowPosition(30, 30);
    g.glutWindow = glutCreateWindow("celtic knots");

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION | GL_MATRIX_MODE);
    glLoadIdentity();

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glOrtho(-100, g.windowWidth+100, g.windowHeight+100, -100, 0, 1);

    glutDisplayFunc(displayFunc);
    glutKeyboardFunc(keyboardFunc);

    glutMainLoop();
    return 0;
}
