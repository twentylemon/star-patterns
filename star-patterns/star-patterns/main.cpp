
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
    std::string display = "4.8.8";
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3d(1.0, 1.0, 1.0);
    g.tilings[display].drawTiling(g.width, g.height);
    glFlush();
    
    std::ofstream svg("../../output.svg");
    svg << "<?xml version=\"1.0\"?>" << std::endl
        << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl
        << "<svg width=\"" << g.windowWidth << "\" height=\"" << g.windowHeight << "\" viewBox=\"-10 -10 20 20\" "
        << "version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;
    g.tilings[display].writeTiling(svg, g.windowWidth, g.windowHeight);
    svg << "</svg>" << std::endl;
    svg.close();
    
}

int main(int argc, char** argv) {
    g.windowWidth = 600;
    g.windowHeight = 600;

    g.width = 20;
    g.height = 20;
    
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

    glOrtho(0, g.width, g.height, 0, 0, 1);

    glutDisplayFunc(displayFunc);

    glutMainLoop();
    return 0;
}
