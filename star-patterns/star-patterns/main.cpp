
#include "main.h"

Global g;

// loads the xml file
void parseXML(const std::string file) {
    using boost::property_tree::ptree;
    std::ifstream in(file);
    ptree tree;
    boost::property_tree::read_xml(in, tree);
    for (ptree::value_type& v : tree.get_child("tiling-library")) {
        if (v.first == "tiling") {
            g.tilings.emplace(v.second.get<std::string>("<xmlattr>.name"), v.second);
        }
    }
}

// glut display function
void displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

int main(int argc, char** argv) {
    g.windowWidth = 600;
    g.windowHeight = 600;
    
    parseXML("tilings/archimedeans.tl");
    parseXML("tilings/hanbury.tl");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(g.windowWidth, g.windowHeight);
    glutInitWindowPosition(30, 30);
    g.glutWindow = glutCreateWindow("star patterns");

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION | GL_MATRIX_MODE);
    glLoadIdentity();

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glOrtho(0, 100, 0, 100, 0, 1);

    glutDisplayFunc(displayFunc);

    glutMainLoop();
    return 0;
}
