
#include "main.h"

Global g;
const std::pair<int,int> NO_SELECTION = std::make_pair(-1, -1);

// return distance between the two points
double dist(double x1, double y1, double x2, double y2) {
    return std::sqrt(std::pow(x1-x2, 2) + std::pow(y1-y2, 2));
}

// glut display function
void displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT);
    g.knot.glRender(g.windowWidth, g.windowHeight);
    if (g.selected != NO_SELECTION) {
        g.knot.renderSelect(g.selected.first, g.selected.second);
    }
    glFlush();
}

/**
    glut keyboard func
    v - save knot as text file
    q - quit
**/
void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
    case 'v':
        g.knot.save("../../knot.txt");
        std::cout << "knot.txt saved!" << std::endl;
        break;
    case 'q':
        glutLeaveMainLoop();
        break;
    }
}

std::pair<int,int> getSelection(int x, int y) {
    GLdouble objX, objY, objZ;
    gluUnProject(x, g.windowHeight - y, 0, g.model, g.proj, g.view, &objX, &objY, &objZ);
    for (int i = 0; i <= g.knot.width(); i++) {
        for (int j = 0; j <= g.knot.height(); j++) {
            if ((i+j)%2 == 0 && dist(i*g.knot.cell_size(), j*g.knot.cell_size(), objX, objY) <= 10.0) {
                return std::make_pair(i, j);
            }
        }
    }
    return NO_SELECTION;
}

// add/remove break markers on clicks
void mouseFunc(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if (g.selected != NO_SELECTION) { // try to add/remove a break marker
            std::pair<int,int> select = getSelection(x, y);
            if (std::abs(g.selected.first - select.first) == 2 && g.selected.second == select.second) {
                g.knot.addMarker(std::min(g.selected.first, select.first), select.second, BreakMarker::East)
                    || g.knot.removeMarker(std::min(g.selected.first, select.first), select.second, BreakMarker::East);
                g.selected = NO_SELECTION;
            }
            else if (g.selected.first == select.first && std::abs(g.selected.second - select.second) == 2) {
                g.knot.addMarker(select.first, std::min(g.selected.second, select.second), BreakMarker::South)
                    || g.knot.removeMarker(select.first, std::min(g.selected.second, select.second), BreakMarker::South);
                g.selected = NO_SELECTION;
            }
            else {
                g.selected = getSelection(x, y);
            }
        }
        else {
            g.selected = getSelection(x, y);
        }
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {

    g.windowWidth = 600;
    g.windowHeight = 600;
    
    g.knot = CelticGrid("../../sample-knot.txt");
    g.knot = CelticGrid("../../border-knot.txt");
    g.knot.set_cell_size(g.windowWidth / g.knot.width());

    g.selected = NO_SELECTION;

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
    
    glGetDoublev(GL_MODELVIEW_MATRIX, g.model);
    glGetDoublev(GL_PROJECTION_MATRIX, g.proj);
    glGetIntegerv(GL_VIEWPORT, g.view);

    glutDisplayFunc(displayFunc);
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunc);

    glutMainLoop();
    return 0;
}
