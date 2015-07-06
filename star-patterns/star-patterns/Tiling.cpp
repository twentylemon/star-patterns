

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


void Tiling::draw() const {
    glBegin(GL_LINE_LOOP);
    for (auto& verts : tiles_.front().verticies()) {
        glVertex2dv(verts.data());
    }
    glEnd();
}
