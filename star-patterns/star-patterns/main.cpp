
#include "main.h"

const std::string ARCH = "tilings/archimedeans.tl";
const std::string HAN  = "tilings/hanbury.tl";

Global g;

// loads the xml file
void parseXML(const std::string file) {
    using boost::property_tree::ptree;
    std::ifstream in(file);
    ptree tree;
    boost::property_tree::read_xml(in, tree);

    const ptree& tilings = tree.get_child("tiling-library").get_child("tiling");
    std::for_each(tilings.begin(), tilings.end(), [](const ptree::value_type& v){
        std::cout << v.first << std::endl;
    });
}

// glut display function
void displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

int main(int argc, char** argv) {
    g.windowWidth = 600;
    g.windowHeight = 600;

    parseXML(HAN);

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
