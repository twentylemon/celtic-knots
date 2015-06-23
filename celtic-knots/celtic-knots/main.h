
#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <iostream>
#include <GL/glut.h>

#include <lemon/collections.h>

typedef struct glob {
    int glutWindow;     // glut window id
    int windowWidth;    // width of glut window
    int windowHeight;   // height of glut window
} Global;


extern Global g;

// glut functions
void displayFunc();

#endif
