
#ifndef CELTICGRID_H
#define CELTICGRID_H

#include <string>
#include <fstream>
#include <iostream>

#include <GL/glut.h>

#include <lemon/collections.h>

#include "BreakMarker.h"
#include "CelticCell.h"

class CelticGrid
{
public:
    CelticGrid() {} // default for Global

    // initializes an empty grid with the given width and height
    CelticGrid(int width, int height);

    // initialzes a grid from the file given
    CelticGrid(std::string inFile);

    // returns the width of the grid, number of cells
    int width() const;

    // returns the height of the grid, number of cells
    int height() const;

    // returns the size of each square cell in pixels
    int cellSize() const;

    // returns the size of the path stroke to use in the knots
    int stroke() const;

    // returns the break markers
    lemon::Vector<BreakMarker>& markers();
    
    // returns the break markers
    const lemon::Vector<BreakMarker>& markers() const;

    // returns the break markers
    lemon::Vector<lemon::Vector<CelticCell>>& cells();
    
    // returns the break markers
    const lemon::Vector<lemon::Vector<CelticCell>>& cells() const;

    // sets the side length of a cell
    void setCellSize(int cellSize);

    // sets the stroke size in the knot paths
    void setStroke(int stroke);

    // writes this knot to the file given
    void save(std::string outFile) const;

    // adds a new marker to the grid
    void addMarker(int x, int y, BreakMarker::Direction direction);
    
    // adds a new marker to the grid
    void addMarker(const BreakMarker& marker);

    // renders this knot on the gl display
    void glRender(int windowWidth, int windowHeight) const;
    
    // writes the svg version of the image to the stream
    friend std::ostream& operator<<(std::ostream& out, const CelticGrid& knot);

private:
    int width_;     // number of cells across
    int height_;    // number of cells high
    int cellSize_;  // side length of a cell for rendering
    int stroke_;    // width of path stroke for rendering

    lemon::Vector<BreakMarker> markers_;                // all of the break markers
    lemon::Vector<lemon::Vector<CelticCell>> cells_;    // all of the cells, including disabled ones
};

#endif
