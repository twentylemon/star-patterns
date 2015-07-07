
#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <iostream>
#include <GL/glut_old.h>
#include <lemon/collections.h>

#include "Tiling.h"

typedef struct global {
    lemon::Vector<Tiling> tilings;
    int currentTiling;      // the selected tiling
    bool displayTiling;     // true to display the actual tiling
    bool displayStar;       // true to display the star pattern
    
    int width;          // width of the ortho
    int height;         // height of the ortho
    double angle;       // contact angle for the star pattern

    int glutWindow;     // glut window id
    int windowWidth;    // width of glut window
    int windowHeight;   // height of glut window
} Global;

extern Global g;

// menu.cpp
void initGLUI();

void parseXML(std::string file);
void save(std::string file);

#endif
