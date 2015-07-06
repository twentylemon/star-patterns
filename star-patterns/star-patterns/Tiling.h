
#ifndef TILING_H
#define TILING_H

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

    // returns true if the two tilings are equal
    bool operator==(const Tiling& rhs) const;

private:
    // parses the ptree, the <tiling> tag in a xml file
    void parseTiling(const boost::property_tree::ptree& tiling);

    lemon::String name_;        // name of this tiling
    lemon::Vector<Tile> tiles_; // the tiles
    lemon::Vector<lemon::Array<double, 2>> translations_;   // translations to fill the plane
};
#endif
