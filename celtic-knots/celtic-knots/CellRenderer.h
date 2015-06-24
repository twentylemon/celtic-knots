
#ifndef CELLRENDERER_H
#define CELLRENDERER_H

#include <iostream>
#include <GL/glut.h>

#include "CelticCell.h"

class CellRenderer
{
public:
    // inits cell size to 0
    CellRenderer() : cell_size_(0.0f), half_size_(0.0f) { }

    // inits the cell size
    CellRenderer(int cellSize, float ribbonSize, float borderSize);

    // returns the cell size
    float cell_size() const { return cell_size_; }

    // returns the cell size div 2
    float half_size() const { return half_size_; }

    // returns the width of the ribbon
    float ribbon_size() const { return ribbon_size_; }

    // returns the width of the ribbon
    float border_size() const { return border_size_; }

    // sets the new cell size and half cell size
    void set_cell_size(float cell_size) {
        cell_size_ = cell_size;
        half_size_ = cell_size / 2.0f;
    }
    
    // sets the new ribbon width size
    void set_ribbon_size(float ribbon_size) { ribbon_size_ = ribbon_size; }

    // sets the new ribbon width size
    void set_border_size(float border_size) { border_size_ = border_size; }

    // renders the cell
    void render(const CelticCell& cell) const;

private:
    void renderPipe() const;


    float cell_size_;   // the cell size
    float half_size_;   // half the cell size

    float ribbon_size_; // width of the ribbon in the cell
    float border_size_; // width of the border on either side of the ribbon
};

#endif
