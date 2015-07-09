

#include <cmath>
#include <sstream>
#include <GL/glut_old.h>
#include <boost/math/constants/constants.hpp>

#include "Tile.h"

using boost::property_tree::ptree;
static const double pi = boost::math::constants::pi<double>();

Tile::Tile()
{
}

Tile::Tile(const ptree& tile)
{
    parseShape(tile);
    parseTransform(tile);
}

void Tile::parseShape(const ptree& tile) {
    const ptree& shape = tile.get_child("shape");
    std::string type = shape.get<std::string>("<xmlattr>.type");
    if (type == "regular") {
        const int n = shape.get<int>("<xmlattr>.sides");
        const double theta = 2.0*pi / n;
        const double phi = 2.0 * std::tan(theta / 2.0);
        addVertex(1.0, -phi/2.0);
        for (int i = 0; i < n - 1; i++) {
            addVertex(lastX() + phi*std::cos(pi/2.0 + theta*i), lastY() + phi*std::sin(pi/2.0 + theta*i));
        }
    }
    else if (type == "polygon") {
        for (const ptree::value_type& v : shape) {
            if (v.first == "vertex") {
                verticies_.emplace_back(v.second.get<double>("<xmlattr>.x"), v.second.get<double>("<xmlattr>.y"));
            }
        }
    }
}

void Tile::parseTransform(const ptree& tile) {
    for (const ptree::value_type& v : tile) {
        if (v.first == "transform") {
            transforms_.emplace_back(0);    // fill with 0
            transforms_.back()[0] = v.second.get<double>("<xmlattr>.a");
            transforms_.back()[4] = v.second.get<double>("<xmlattr>.b");
            transforms_.back()[12] = v.second.get<double>("<xmlattr>.c");
            transforms_.back()[1] = v.second.get<double>("<xmlattr>.d");
            transforms_.back()[5] = v.second.get<double>("<xmlattr>.e");
            transforms_.back()[13] = v.second.get<double>("<xmlattr>.f");
            transforms_.back()[10] = 1.0;
            transforms_.back()[15] = 1.0;   // note: opengl is column major
        }
    }
}

void Tile::addVertex(double x, double y) {
    verticies_.emplace_back(x, y);
}

double Tile::lastX() const {
    return verticies_.back()[0];
}

double Tile::lastY() const {
    return verticies_.back()[1];
}

lemon::Vector<lemon::util::Point<2>>& Tile::verticies() {
    return verticies_;
}

const lemon::Vector<lemon::util::Point<2>>& Tile::verticies() const {
    return verticies_;
}

lemon::Vector<lemon::Array<double, 16>>& Tile::transforms() {
    return transforms_;
}

const lemon::Vector<lemon::Array<double, 16>>& Tile::transforms() const {
    return transforms_;
}

bool Tile::operator==(const Tile& rhs) const {
    return verticies_ == rhs.verticies_ && transforms_ == rhs.transforms_;
}

void Tile::draw() const {
    transforms_.each([this](const lemon::Array<double, 16>& matrix){
        glPushMatrix();
        glMultMatrixd(matrix.data());
        glBegin(GL_LINE_LOOP);
        verticies_.each([](const lemon::util::Point<2>& point){
            glVertex2dv(point.data());
        });
        glEnd();
        glPopMatrix();
    });
}

std::ostream& operator<<(std::ostream& svg, const Tile& tile) {
    tile.transforms().each([&](const lemon::Array<double, 16>& matrix){
        svg << "<polygon class=\"tile\" " << Tile::svg_transform(matrix) << " points=\"";
        tile.verticies().dump(svg, " ");
        svg << "\"/>";
    });
    return svg;
}

std::string Tile::to_string(std::string polyclass) const {
    std::stringstream str;
    transforms().each([&](const lemon::Array<double, 16>& matrix){
        str << "<polygon ";
        if (!polyclass.empty()) {
            str << "class=\"" << polyclass << "\" ";
        }
        str << Tile::svg_transform(matrix) << " points=\"";
        verticies().dump(str, " ");
        str << "\"/>";
    });
    return str.str();
}


std::string Tile::svg_transform(const lemon::Array<double, 16>& matrix) {
    std::stringstream str;
    str << "transform=\"matrix("
        << matrix[0] << " " << matrix[1] << " " << matrix[4] << " " << matrix[5] << " " << matrix[12] << " " << matrix[13]
        << ")\"";
    return str.str();
}
