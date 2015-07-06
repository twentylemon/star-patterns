

#include "Tile.h"

using boost::property_tree::ptree;

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
        const double pi = boost::math::constants::pi<double>();
        for (int i = 0; i < n; i++) {
            verticies_.emplace_back();
            verticies_[i][0] = std::cos(2.0*pi*i/n);
            verticies_[i][1] = std::sin(2.0*pi*i/n);
        }
    }
    else if (type == "polygon") {
        for (const ptree::value_type& v : shape) {
            if (v.first == "vertex") {
                verticies_.emplace_back();
                verticies_.back()[0] = v.second.get<double>("<xmlattr>.x");
                verticies_.back()[1] = v.second.get<double>("<xmlattr>.y");
            }
        }
    }
}

void Tile::parseTransform(const ptree& tile) {
    for (const ptree::value_type& v : tile) {
        if (v.first == "transform") {
            transforms_.emplace_back(0);    // fill with 0
            transforms_.back()[0] = v.second.get<double>("<xmlattr>.a");
            transforms_.back()[1] = v.second.get<double>("<xmlattr>.b");
            transforms_.back()[3] = v.second.get<double>("<xmlattr>.c");
            transforms_.back()[4] = v.second.get<double>("<xmlattr>.d");
            transforms_.back()[5] = v.second.get<double>("<xmlattr>.e");
            transforms_.back()[7] = v.second.get<double>("<xmlattr>.f");
            transforms_.back()[10] = 1.0;
            transforms_.back()[15] = 1.0;
        }
    }
}

lemon::Vector<lemon::Array<double, 2>>& Tile::verticies() {
    return verticies_;
}

const lemon::Vector<lemon::Array<double, 2>>& Tile::verticies() const {
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
