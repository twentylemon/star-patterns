

#include "main.h"

#include <fstream>
#include <GL/glui.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

static GLUI_Listbox* libraryList = nullptr;

using boost::property_tree::ptree;

void initGLUI() {
    GLUI* glui = GLUI_Master.create_glui("parameters");
    GLUI_Panel* tiling = glui->add_panel("tiling");
    libraryList = glui->add_listbox_to_panel(tiling, "select", &g.currentTiling);
    glui->add_spinner_to_panel(tiling, "width", GLUI_SPINNER_INT, &g.width);
    glui->add_spinner_to_panel(tiling, "height", GLUI_SPINNER_INT, &g.height);
    glui->set_main_gfx_window(g.glutWindow);
    GLUI_Master.set_glutIdleFunc(NULL);
}

// loads the xml file
void parseXML(std::string file) {
    std::ifstream in(file);
    ptree tree;
    boost::property_tree::read_xml(in, tree);
    for (ptree::value_type& v : tree.get_child("tiling-library")) {
        if (v.first == "tiling") {
            libraryList->add_item(g.tilings.size(), v.second.get<std::string>("<xmlattr>.name").c_str());
            g.tilings.emplace_back(v.second);
        }
    }
}

// saves the current tiling into the svg
void save(std::string file) {
    std::cout << "saving " << file << "...";
    std::ofstream svg(file);
    svg << "<?xml version=\"1.0\"?>" << std::endl
        << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl
        << "<svg width=\"" << g.windowWidth << "\" height=\"" << g.windowHeight << "\" viewBox=\"-10 -10 20 20\" "
        << "version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;
    g.tilings[g.currentTiling].writeTiling(svg, g.width, g.height);
    svg << "</svg>" << std::endl;
    svg.close();
    std::cout << "done!" << std::endl;
}
