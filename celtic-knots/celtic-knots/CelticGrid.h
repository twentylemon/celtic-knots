
#ifndef CELTICGRID_H
#define CELTICGRID_H

#include <string>
#include <fstream>
#include <iostream>

#include <GL/glut.h>

#include <lemon/collections.h>

#include "BreakMarker.h"
#include "CelticCell.h"
#include "CellRenderer.h"

class CelticGrid
{
public:
    CelticGrid() {} // default for Global

    // initializes an empty grid with the given width and height
    CelticGrid(int width, int height);

    // initialzes a grid from the file given
    CelticGrid(std::string inFile);

    // returns the width of the grid, number of cells
    int width() const { return width_; }

    // returns the height of the grid, number of cells
    int height() const { return height_; }

    // returns the size of each square cell in pixels
    int cell_size() const { return cell_size_; }

    // returns the size of the path stroke to use in the knots
    int stroke() const { return stroke_; }

    // returns the size to render the grid points as
    float point_size() const { return point_size_; }

    // returns the size to render the break marker lines as
    float marker_size() const { return marker_size_; }
    
    // returns true if will render the grid
    bool display_grid() const { return display_grid_; }

    // returns true if will render the break markers
    bool display_markers() const { return display_markers_; }

    // returns the break markers
    lemon::Vector<BreakMarker>& markers() { return markers_; }
    
    // returns the break markers
    const lemon::Vector<BreakMarker>& markers() const { return markers_; }

    // returns the cells
    lemon::Vector<lemon::Vector<CelticCell>>& cells() { return cells_; }
    
    // returns the cells
    const lemon::Vector<lemon::Vector<CelticCell>>& cells() const { return cells_; }

    CellRenderer& cell_renderer() { return cell_renderer_; }

    const CellRenderer& cell_renderer() const { return cell_renderer_; }



    // sets the side length of a cell
    void set_cell_size(int cell_size) {
        cell_size_ = cell_size;
        cell_renderer_.set_cell_size((float)cell_size);
    }

    // sets the stroke size in the knot paths
    void set_stroke(int stroke) { stroke_ = stroke; }

    // sets the new size to render grid points as
    void set_point_size(float point_size) { point_size_ = point_size; }
    
    // sets the new size to render the break marker lines as
    void set_marker_size(float marker_size) { marker_size_ = marker_size; }
    
    // sets whether or not to display the grid
    void set_display_grid(bool display_grid) { display_grid_ = display_grid; }
    
    // sets whether or not to display the break markers
    void set_display_markers(bool display_markers) { display_markers_ = display_markers; }



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
    void init();

    // renders the grid
    void renderGrid() const;

    // renders the break markers
    void renderMarkers() const;

    int width_;     // number of cells across
    int height_;    // number of cells high
    int cell_size_; // side length of a cell for rendering
    int stroke_;    // width of path stroke for rendering

    float point_size_;  // size of the points in the grid
    float marker_size_; // width of the break marker lines

    bool display_grid_;
    bool display_markers_;

    CellRenderer cell_renderer_;

    lemon::Vector<BreakMarker> markers_;                // all of the break markers
    lemon::Vector<lemon::Vector<CelticCell>> cells_;    // all of the cells, including disabled ones
};

#endif
