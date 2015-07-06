

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
    name_ = tiling.get<lemon::String>("<xmlattr>.name");

    // translation vectors
    const ptree& trans = tiling.get_child("translations").get_child("vector");
    std::for_each(trans.begin(), trans.end(), [this](const ptree::value_type& v){
        translations_.emplace_back();
        translations_.back()[0] = v.second.get<double>("<xmlattr>.x");
        translations_.back()[1] = v.second.get<double>("<xmlattr>.y");
    });

    // tiles
    const ptree& tiles = tiling.get_child("tile");
    std::for_each(tiles.begin(), tiles.end(), [this](const ptree::value_type& v){
        tiles_.emplace_back(v.second);
    });
}

const lemon::String& Tiling::name() const {
    return name_;
}
