

#include "main.h"

#include <fstream>
#include <GL/glui.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/math/constants/constants.hpp>

void save(int);

GLUI_Listbox* libraryList = nullptr;
GLUI_StaticText* saving = nullptr;

using boost::property_tree::ptree;

void color_spinners(GLUI_Node* parent, lemon::Array<int,3>& var) {
    (new GLUI_Spinner(parent, "red", &var[0]))->set_int_limits(0, 255);
    (new GLUI_Spinner(parent, "green", &var[1]))->set_int_limits(0, 255);
    (new GLUI_Spinner(parent, "blue", &var[2]))->set_int_limits(0, 255);
}

void initGLUI() {
    GLUI* glui = GLUI_Master.create_glui("star pattern svg parameters");

    GLUI_Panel* tiling = new GLUI_Panel(glui, "tiling");
    libraryList = new GLUI_Listbox(tiling, "select", &g.currentTiling);
    new GLUI_Checkbox(tiling, "display", &g.displayTiling);
    color_spinners(new GLUI_Panel(tiling, "stroke color"), g.tile_stroke_color);
    GLUI_Panel* tile_fill = new GLUI_Panel(tiling, "fill color");
    color_spinners(tile_fill, g.tile_fill_color);
    new GLUI_Checkbox(tile_fill, "transparent", &g.tile_fill_transparent);
    (new GLUI_Spinner(tiling, "stroke width", &g.tile_stroke_width))->set_float_limits(0.0f, 1.0f);

    glui->add_column();
    GLUI_Panel* star = new GLUI_Panel(glui, "star");
    new GLUI_Checkbox(star, "display", &g.displayStar);
    new GLUI_Checkbox(star, "interlace", &g.interlace);
    (new GLUI_Spinner(star, "angle", &g.angle))->set_float_limits(0.0f, 180.0f);
    color_spinners(new GLUI_Panel(star, "stroke color"), g.star_stroke_color);
    GLUI_Panel* star_fill = new GLUI_Panel(star, "fill color");
    color_spinners(star_fill, g.star_fill_color);
    new GLUI_Checkbox(star_fill, "transparent", &g.star_fill_transparent);
    (new GLUI_Spinner(star, "stroke width", &g.star_stroke_width))->set_float_limits(0.0f, 1.0f);

    glui->add_column();
    GLUI_Panel* etc = new GLUI_Panel(glui, "etc");
    new GLUI_Spinner(etc, "width", GLUI_SPINNER_INT, &g.width);
    new GLUI_Spinner(etc, "height", GLUI_SPINNER_INT, &g.height);
    new GLUI_Button(etc, "save svg", -1, save);
    saving = new GLUI_StaticText(etc, "");
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
    in.close();
}

std::string rgb(const lemon::Array<int,3>& ary) {
    return "rgb("+std::to_string(ary[0])+","+std::to_string(ary[1])+","+std::to_string(ary[2])+")";
}

std::string fill(const lemon::Array<int,3>& ary, int transparent) {
    if (transparent) {
        return "transparent";
    }
    return rgb(ary);
}

float to_rads(float degrees) {
    return degrees * boost::math::constants::pi<float>() / 180.0f;
}

// saves the current tiling into the svg
void save(std::string file) {
    saving->set_text(("saving " + file + "...").c_str());
    std::ofstream svg(file);
    svg << "<?xml version=\"1.0\"?>" << std::endl
        << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl
        << "<svg width=\"" << g.windowWidth << "\" height=\"" << g.windowHeight << "\" viewBox=\"0 0 " << g.width << " " << g.height << "\" "
        << "version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;

    svg << "<style type=\"text/css\"><![CDATA[" << std::endl
        << "polygon.tile { stroke:" << rgb(g.tile_stroke_color) << ";fill:" << fill(g.tile_fill_color, g.tile_fill_transparent) << ";stroke-width:" << g.tile_stroke_width << "; }" << std::endl
        << "polygon.star { stroke:" << rgb(g.star_stroke_color) << ";fill:" << fill(g.star_fill_color, g.star_fill_transparent) << ";stroke-width:" << g.star_stroke_width << "; }" << std::endl
        << "]]></style>" << std::endl;

    g.tilings[g.currentTiling].write(svg, g.width, g.height, g.displayTiling, g.displayStar, to_rads(g.angle), g.interlace);
    svg << "</svg>" << std::endl;
    svg.close();
    saving->set_text(("wrote " + file + "!").c_str());
}

void save(int) {
    save("../../output.svg");
}
