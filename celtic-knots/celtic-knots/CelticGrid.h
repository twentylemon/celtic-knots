
#ifndef CELTICGRID_H
#define CELTICGRID_H

#include <string>
#include <fstream>

#include <GL/glut.h>

#include <lemon/collections.h>

#include "BreakMarker.h"
#include "CelticCell.h"
#include "CellRenderer.h"

// the knot master class, handles loading, saving and rendering
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
    int cell_size() const;

    // returns the size of the path stroke to use in the knots
    int stroke() const;

    // returns the size to render the grid points as
    float point_size() const;

    // returns the size to render the break marker lines as
    float marker_size() const;
    
    // returns true if will render the grid
    bool display_grid() const;

    // returns true if will render the break markers
    bool display_markers() const;

    // returns the break markers
    lemon::Vector<BreakMarker>& markers();
    
    // returns the break markers
    const lemon::Vector<BreakMarker>& markers() const;

    // returns the cells
    lemon::Vector<lemon::Vector<CelticCell>>& cells();
    
    // returns the cells
    const lemon::Vector<lemon::Vector<CelticCell>>& cells() const;

    // returns the cell renderer
    CellRenderer& cell_renderer();
    
    // returns the cell renderer
    const CellRenderer& cell_renderer() const;



    // sets the side length of a cell
    void set_cell_size(int cell_size);

    // sets the stroke size in the knot paths
    void set_stroke(int stroke);

    // sets the new size to render grid points as
    void set_point_size(float point_size);
    
    // sets the new size to render the break marker lines as
    void set_marker_size(float marker_size);
    
    // sets whether or not to display the grid
    void set_display_grid(bool display_grid);
    
    // sets whether or not to display the break markers
    void set_display_markers(bool display_markers);



    // writes this knot to the file given
    void save(std::string outFile) const;

    // adds a new marker to the grid
    bool addMarker(int x, int y, BreakMarker::Direction direction);

    // removes the break marker
    bool removeMarker(int x, int y, BreakMarker::Direction direction);

    // updates the cell neighbours given the marker flag
    void handleCell(int x, int y, BreakMarker::Direction direction, bool marker);



    // renders this knot on the gl display
    void glRender(int windowWidth, int windowHeight) const;

    // renders the point as yellow
    void renderSelect(int x, int y) const;

private:
    // ctor helper, after width and height have been init
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

    bool display_grid_;     // whether or not to display the grid points
    bool display_markers_;  // whether or not to display the break markers

    CellRenderer cell_renderer_;    // our cell renderer

    lemon::Vector<BreakMarker> markers_;                // all of the break markers
    lemon::Vector<lemon::Vector<CelticCell>> cells_;    // all of the cells, including disabled ones
};

#endif
