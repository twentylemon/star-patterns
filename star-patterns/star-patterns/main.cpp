
#include "main.h"

Global g;

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
        g.tilings[g.currentTiling].draw(g.width, g.height);
    }
    glFlush();
}

// entry point
int main(int argc, char** argv) {
    g.windowWidth = 600;
    g.windowHeight = 600;
    
    g.width = 10;
    g.height = 10;

    g.currentTiling = 0;
    g.displayTiling = 0;
    g.displayStar = 1;
    g.angle = 60.0f;
    g.interlace = 0;
    
    g.tile_stroke_color[0] = g.tile_stroke_color[1] = 0;
    g.tile_stroke_color[2] = 255;
    g.tile_fill_color.fill(255);
    g.tile_fill_transparent = 1;
    g.tile_stroke_width = 0.125f;

    g.bkgd_color.fill(255);

    g.star_stroke_color.fill(0);
    g.star_fill_color.fill(128);
    g.star_fill_transparent = 0;
    g.star_stroke_width = 0.1f;

    g.ribbon_color.fill(0);
    g.ribbon_color[0] = 255;
    g.ribbon_size = 0.075f;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(g.windowWidth, g.windowHeight);
    glutInitWindowPosition(30, 30);
    g.glutWindow = glutCreateWindow("tiling");

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
