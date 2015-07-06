

#include "Tiling.h"

using boost::property_tree::ptree;


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
            translations_.emplace_back();
            translations_.back()[0] = v.second.get<double>("<xmlattr>.x");
            translations_.back()[1] = v.second.get<double>("<xmlattr>.y");
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

void Tiling::drawTiling(int width, int height) const {
    lemon::Vector<std::pair<int,int>> visited;
    drawTiling(width, height, 0, 0, 0, 0, visited);
}

void Tiling::drawTiling(int width, int height, double x, double y, int dx, int dy, lemon::Vector<std::pair<int,int>>& visited) const {
    if (x < -width/2 || x > 3*width/2 || y < -height/2 || y > 3*height/2 || visited.contains(std::make_pair(dx, dy))) {
        return;
    }
    
    visited.emplace_back(dx, dy);
    glPushMatrix();
    glTranslated(x, y, 0.0);
    tiles_.each([](const Tile& tile){ tile.draw(); });
    glPopMatrix();

    drawTiling(width, height, x+translations_[0][0], y+translations_[0][1], dx+1, dy, visited);
    drawTiling(width, height, x-translations_[0][0], y-translations_[0][1], dx-1, dy, visited);
    drawTiling(width, height, x+translations_[1][0], y+translations_[1][1], dx, dy+1, visited);
    drawTiling(width, height, x-translations_[1][0], y-translations_[1][1], dx, dy-1, visited);
}


void Tiling::writeTiling(std::ostream& svg, int width, int height) const {
    tiles_.dump(svg);
}
