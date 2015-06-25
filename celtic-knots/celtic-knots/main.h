
#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <utility>
#include <iostream>
#include <GL/glut.h>

#include <lemon/collections.h>

#include "CelticGrid.h"

typedef struct glob {
    CelticGrid knot;

    std::pair<int, int> selected;

    int glutWindow;     // glut window id
    int windowWidth;    // width of glut window
    int windowHeight;   // height of glut window
    GLdouble model[16]; // modelview matrix
    GLdouble proj[16];  // projection matrix
    GLint view[4];      // viewport matrix
} Global;


extern Global g;

// glut functions
void displayFunc();

#endif
