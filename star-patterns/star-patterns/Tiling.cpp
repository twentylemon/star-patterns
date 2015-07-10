

#include "Tiling.h"
#include <functional>
#include <GL/glut_old.h>
#include <boost/math/constants/constants.hpp>

using boost::property_tree::ptree;

const double PI = boost::math::constants::pi<double>();

typedef lemon::util::Point<2> Point;


Tiling::Tiling()
{
}

Tiling::Tiling(const ptree& tiling)
{
    parseTiling(tiling);
}


void Tiling::parseTiling(const ptree& tiling) {
    name_ = tiling.get<std::string>("<xmlattr>.name");

    // translation vectors
    for (const ptree::value_type& v : tiling.get_child("translations")) {
        if (v.first == "vector") {
            translations_.emplace_back(v.second.get<double>("<xmlattr>.x"), v.second.get<double>("<xmlattr>.y"));
        }
    }

    // tiles
    for (const ptree::value_type& v : tiling) {
        if (v.first == "tile") {
            tiles_.emplace_back(v.second);
        }
    }
}

const lemon::String& Tiling::name() const {
    return name_;
}

bool Tiling::operator==(const Tiling& rhs) const {
    return name_ == rhs.name_ && translations_ == rhs.translations_ && tiles_ == rhs.tiles_;
}

void Tiling::draw(int width, int height) const {
    draw(width, height, 0, 0, 0, 0, lemon::Vector<std::pair<int,int>>());
}

void Tiling::draw(int width, int height, double x, double y, int dx, int dy, lemon::Vector<std::pair<int,int>>& visited) const {
    if (x < -width/2 || x > 3*width/2 || y < -height/2 || y > 3*height/2 || visited.contains(std::make_pair(dx, dy))) {
        return;
    }
    
    visited.emplace_back(dx, dy);
    glPushMatrix();
    glTranslated(x, y, 0.0);
    tiles_.each(std::mem_fn(&Tile::draw));
    glPopMatrix();

    draw(width, height, x+translations_[0][0], y+translations_[0][1], dx+1, dy, visited);
    draw(width, height, x-translations_[0][0], y-translations_[0][1], dx-1, dy, visited);
    draw(width, height, x+translations_[1][0], y+translations_[1][1], dx, dy+1, visited);
    draw(width, height, x-translations_[1][0], y-translations_[1][1], dx, dy-1, visited);
}



Point Tiling::rotateAndScale(const Point& p, float angle, double scale, const Point& pivot) const {
    Point r = (p - pivot).scale(scale);
    return Point(std::cos(angle)*r[0] - std::sin(angle)*r[1], std::sin(angle)*r[0] + std::cos(angle)*r[1]) + pivot;
}

void Tiling::write(std::ostream& svg, int width, int height, int includeTile, int includeStar, float angle, int interlace) const {
    write(svg, width, height, 0, 0, 0, 0, lemon::Vector<std::pair<int,int>>(), includeTile, includeStar, angle, interlace);
}

void Tiling::write(std::ostream& svg, int width, int height, double x, double y, int dx, int dy, lemon::Vector<std::pair<int,int>>& visited, int includeTile, int includeStar, float angle, int interlace) const {
    if (x < -width/2 || x > 3*width/2 || y < -height/2 || y > 3*height/2 || visited.contains(std::make_pair(dx, dy))) {
        return;
    }
    
    /*
    double w = translations_.fold([](double s, const Point& p){ return std::max(s, std::fabs(p[0])); }, 0.0);
    double h = translations_.fold([](double s, const Point& p){ return std::max(s, std::fabs(p[1])); }, 0.0);
    svg << "<defs>" << std::endl
        << "  <pattern id=\"pattern\" x=\"0\" y=\"0\" width=\"" << w << "\" height=\"" << h << "\" patternUnits=\"userSpaceOnUse\">"
        << std::endl << "    ";
    if (includeTile != 0) {
        tiles_.dump(svg, "");
    }
    if (includeStar != 0) {
        tiles_.each([this,&svg,angle,interlace,dx,dy](const Tile& tile){
            writeStar(svg, tile, angle, interlace, dx, dy);
        });
    }
    svg << "  </pattern>" << std::endl << "</defs>" << std::endl
        << "<rect x=\"0\" y=\"0\" width=\"" << width << "\" height=\"" << height << "\" fill=\"url(#pattern)\"/>" << std::endl;
    */
    visited.emplace_back(dx, dy);
    svg << "<g transform=\"translate(" << x << " " << y << ")\">" << std::endl;
    if (includeTile != 0) {
        tiles_.dump(svg, "");
    }
    if (includeStar != 0) {
        tiles_.each([this,&svg,angle,interlace,dx,dy](const Tile& tile){
            writeStar(svg, tile, angle, interlace, dx, dy);
        });
    }
    svg << "</g>" << std::endl;
    write(svg, width, height, x+translations_[0][0], y+translations_[0][1], dx+1, dy, visited, includeTile, includeStar, angle, interlace);
    write(svg, width, height, x-translations_[0][0], y-translations_[0][1], dx-1, dy, visited, includeTile, includeStar, angle, interlace);
    write(svg, width, height, x+translations_[1][0], y+translations_[1][1], dx, dy+1, visited, includeTile, includeStar, angle, interlace);
    write(svg, width, height, x-translations_[1][0], y-translations_[1][1], dx, dy-1, visited, includeTile, includeStar, angle, interlace);
}

void Tiling::writeStar(std::ostream& svg, const Tile& tile, float angle, int interlace, int dx, int dy) const {
    int id = 0;
    Point center = tile.verticies().sum().scale(1.0 / tile.verticies().size());
    tile.transforms().each([&svg,&tile,&center,angle,interlace,this,dx,dy,&id](const lemon::Array<double, 16>& matrix){
        lemon::Vector<Point> star;
        lemon::Vector<lemon::Vector<Point>> ribbons;
        for (unsigned i = 0; i < tile.verticies().size(); i++) {
            const Point& p1 = tile.verticies()[i];
            const Point& p2 = tile.verticies()[(i+1) % tile.verticies().size()];
            const Point& p3 = tile.verticies()[(i+2) % tile.verticies().size()];
            Point mid12 = p1.between(p2);
            Point mid23 = p2.between(p3);
            Point vec12 = p1 - p2;
            Point vec32 = p3 - p2;
            double a = vec12.magnitude() / 2.0;
            double b = vec32.magnitude() / 2.0;
            double theta = std::acos(vec12.dot(vec32) / (4.0*a*b));
            double phi = 2.0*PI - theta - 2.0*(double)angle;
            double gamma = std::atan(a*std::sin(phi) / (b + a*std::cos(phi)));
            double scale = std::fabs(gamma) < 0.01 ? 1.0 : std::sin(PI - gamma - (double)angle) / std::sin(gamma);
            if (i == 0) {
                star.push_back(mid12);
            }
            Point contact = rotateAndScale(p2, angle, scale, mid12);
            star.push_back(contact);
            star.push_back(mid23);
            if (interlace) {
                ribbons.emplace_back();
                ribbons.back().push_back(rotateAndScale(p2, angle + PI, 1.0, mid12));
                ribbons.back().push_back(contact);
                ribbons.back().push_back(rotateAndScale(p2, PI - angle, 1.0, mid23));
            }
        }
        
        // define the transformation group and clip path
        svg << "  <defs><clipPath id=\"" << dx << "-" << dy << "-" << id << "\">";
        dump(svg, tile.verticies());
        svg << "</clipPath></defs>" << std::endl;
        svg << "  <g " << Tile::svg_transform(matrix) << " clip-path=\"url(#" << dx << "-" << dy << "-" << id << ")\">" << std::endl << "    ";
        id++;

        if (interlace) {
            dump(svg, star, "fill");
            ribbons.each([this,&svg](const lemon::Vector<Point>& ribbon){
                dump(svg, ribbon, "ribbon", "polyline");
                dump(svg, ribbon, "interlace", "polyline");
            });
            lemon::Vector<Point> clip;
            clip.push_back(tile.verticies()[0]);
            clip.push_back(tile.verticies()[1]);
            clip.push_back(center);
            svg << std::endl << "    <defs><clipPath id=\"" << dx << "-" << dy << "-" << id << "\" clip-path=\"url(#" << dx << "-" << dy << "-" << (id-1) << ")\">";
            dump(svg, clip);
            svg << "</clipPath></defs>" << std::endl;
            svg << "    <g clip-path=\"url(#" << dx << "-" << dy << "-" << id << ")\">" << std::endl << "      ";
            dump(svg, ribbons[0], "ribbon", "polyline");
            dump(svg, ribbons[0], "interlace", "polyline");
            svg << std::endl << "    </g>";
            id++;
        }
        else {
            dump(svg, star, "ribbon");
            dump(svg, star, "star");
        }
        svg << std::endl << "  </g>" << std::endl;
    });
}

void Tiling::dump(std::ostream& svg, const lemon::Vector<Point>& points, std::string css_class, std::string poly) const {
    if (css_class == "") {
        svg << "<" << poly << " points=\"";
    }
    else {
        svg << "<" << poly << " class=\"" << css_class << "\" points=\"";
    }
    points.dump(svg, " ");
    svg << "\"/>";
}
