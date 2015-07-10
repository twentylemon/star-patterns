
#ifndef TILING_H
#define TILING_H

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <lemon/collections.h>

#include "Tile.h"

class Tiling
{
public:
    // default empty constructor
    Tiling();

    // loads the ptree into this tiling
    Tiling(const boost::property_tree::ptree& tiling);

    // returns the name of this tiling
    const lemon::String& name() const;

    // draws the tiling to fill the screen from (0,0) to (width,height)
    void draw(int width, int height) const;

    // writes this tiling into the svg file
    void write(std::ostream& svg, int width, int height, int includeTile, int includeStar, float angle, int interlace) const;

    // returns true if the two tilings are equal
    bool operator==(const Tiling& rhs) const;

private:
    // parses the ptree, the <tiling> tag in a xml file
    void parseTiling(const boost::property_tree::ptree& tiling);

    // draws the tiling to fill the screen from (x,y) to (width,height)
    void draw(int width, int height, double x, double y, int dx, int dy, lemon::Vector<std::pair<int,int>>& visited) const;

    void write(std::ostream& svg, int width, int height, double x, double y, int dx, int dy, lemon::Vector<std::pair<int,int>>& visited, int includeTile, int includeStar, float angle, int interlace) const;
    void writeStar(std::ostream& svg, const Tile& tile, float angle, int interlace, int dx, int dy) const;

    lemon::util::Point<2> rotateAndScale(const lemon::util::Point<2>& p, float angle, double scale, const lemon::util::Point<2>& pivot) const;

    void dump(std::ostream& svg, const lemon::Vector<lemon::util::Point<2>>& points, std::string css_class = "", std::string poly = "polygon") const;

    lemon::String name_;        // name of this tiling
    lemon::Vector<Tile> tiles_; // the tiles
    lemon::Vector<lemon::util::Point<2>> translations_; // translations to fill the plane
};
#endif
