
#include "main.h"
#include <boost/math/constants/constants.hpp>

Global g;

// returns the angle in radians
double to_rads(double degrees) {
    return degrees * boost::math::constants::pi<double>() / 180.0;
}

// glut keyboard function
void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
    case 's':
        save("../../output.svg");
        break;
    }
}

// glut display function
void displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    glOrtho(0, g.width, g.height, 0, 0, 1);

    glColor3d(1.0, 1.0, 1.0);
    if (g.currentTiling >= 0 && g.currentTiling < (int)g.tilings.size()) {
        if (g.displayTiling) {
            g.tilings[g.currentTiling].drawTiling(g.width, g.height);
        }
        if (g.displayStar) {
            g.tilings[g.currentTiling].drawStar(g.width, g.height, g.angle);
        }
    }
    glFlush();
}

// entry point
int main(int argc, char** argv) {
    g.windowWidth = 600;
    g.windowHeight = 600;
    
    g.width = 20;
    g.height = 20;

    g.currentTiling = 0;
    g.displayTiling = true;
    g.displayStar = true;
    g.angle = to_rads(60.0);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(g.windowWidth, g.windowHeight);
    glutInitWindowPosition(30, 30);
    g.glutWindow = glutCreateWindow("star patterns");

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION | GL_MATRIX_MODE);
    glLoadIdentity();

    glOrtho(0, g.width, g.height, 0, 0, 1);

    glutDisplayFunc(displayFunc);
    glutKeyboardFunc(keyboardFunc);

    initGLUI();

    parseXML("tilings/archimedeans.tl");
    parseXML("tilings/hanbury.tl");

    glutMainLoop();
    return 0;
}
