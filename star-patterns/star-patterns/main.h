
#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <fstream>
#include <iostream>
#include <GL/glut.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <lemon/collections.h>

typedef struct glob {
    int glutWindow;     // glut window id
    int windowWidth;    // width of glut window
    int windowHeight;   // height of glut window
} Global;

extern Global g;

#endif
