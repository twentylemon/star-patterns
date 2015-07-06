
#ifndef TILE_H
#define TILE_H

#include <cmath>
#include <iostream>
#include <GL/glut.h>

#include <boost/math/constants/constants.hpp>
#include <boost/property_tree/ptree.hpp>
#include <lemon/collections.h>

/*
    one of two shape formats are possible:
    <shape type="regular" sides="n"/>
     - this specifies a regular n-gon in the unit circle with the center of an edge on (1,0)

    <shape type="polygon">
        <vertex x="x" y="y"/>
    </shape>
     - this specifies the polygon verticies in the child nodes

     each tile also has a series of <transform> tags
     <transform a="a" b="b" c="c" d="d" e="e" f="f"/>
      - these specify the number of times the tile is drawn and which transformation
        is applied before drawing in each case
      - a-f are entries in the 3x3 affine matrix
*/


// a <tile> tag in the .tl format
class Tile
{
public:
    // default empty constructor
    Tile();

    // construct a regular n-gon, tile is the <tile> ptree for this shape
    Tile(const boost::property_tree::ptree& tile);

    // returns the verticies
    lemon::Vector<lemon::Array<double, 2>>& verticies();

    // returns the verticies
    const lemon::Vector<lemon::Array<double, 2>>& verticies() const;

    // returns the list of transformation matricies to apply before drawing
    lemon::Vector<lemon::Array<double, 16>>& transforms();

    // returns the list of transformation matricies to apply before drawing
    const lemon::Vector<lemon::Array<double, 16>>& transforms() const;

    // returns true if the two tiles are equal
    bool operator==(const Tile& rhs) const;

    // draws this tile set onto the gl canvas
    void draw() const;

    // writes this tile to the svg
    friend std::ostream& operator<<(std::ostream& svg, const Tile& tile);

private:
    // reads and populations this tiles verticies from the ptree
    void parseShape(const boost::property_tree::ptree& tile);

    // reads and populates this tiles transformations from the ptree
    void parseTransform(const boost::property_tree::ptree& tile);

    // adds a new vertex
    void addVertex(double x, double y);

    double lastX() const;
    double lastY() const;

    lemon::Vector<lemon::Array<double, 2>> verticies_;      // verticies of the shape
    lemon::Vector<lemon::Array<double, 16>> transforms_;    // transformation matricies
};
#endif
