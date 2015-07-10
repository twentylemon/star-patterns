
#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <iostream>
#include <GL/glut_old.h>
#include <lemon/collections.h>
#include <lemon/util/Point.h>

#include "Tiling.h"

typedef struct global {
    lemon::Vector<Tiling> tilings;
    int currentTiling;      // the selected tiling
    int displayTiling;      // true to display the actual tiling
    int displayStar;        // true to display the star pattern
    
    int width;          // width of the ortho
    int height;         // height of the ortho
    float angle;        // contact angle for the star pattern
    int interlace;      // true to include interlacing star pattern

    lemon::Array<int,3> tile_stroke_color;
    lemon::Array<int,3> tile_fill_color;
    int tile_fill_transparent;
    float tile_stroke_width;
    
    lemon::Array<int,3> bkgd_color;
    lemon::Array<int,3> star_stroke_color;
    lemon::Array<int,3> star_fill_color;
    int star_fill_transparent;
    float star_stroke_width;
    
    lemon::Array<int,3> ribbon_color;
    float ribbon_size;

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
