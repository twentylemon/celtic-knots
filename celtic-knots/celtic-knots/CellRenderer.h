
#ifndef CELLRENDERER_H
#define CELLRENDERER_H

#include <string>
#include <fstream>
#include <GL/glut.h>
#include <lemon/collections.h>

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

    // sets the ribbon color
    void set_ribbon_color(float red, float green, float blue) {
        ribbon_color_[0] = red;
        ribbon_color_[1] = green;
        ribbon_color_[2] = blue;
    }

    // sets the border color
    void set_border_color(float red, float green, float blue) {
        border_color_[0] = red;
        border_color_[1] = green;
        border_color_[2] = blue;
    }


    // renders the cell and writes the svg to the stream
    void render(const CelticCell& cell, std::ostream& svg) const;

    // renders the cell
    void render(const CelticCell& cell) const;

    

private:
    // renders odd cells
    void renderOddCell(const CelticCell& cell) const;

    // renders even cells
    void renderEvenCell(const CelticCell& cell) const;

    // handles gl calls for starting the border strip
    void startBorder() const;

    // handles gl calls for starting the ribbon strip
    void startRibbon() const;

    // sets up the clipping box around this cell
    void setUpScissor(float objX, float objY) const;

    // displays whatever is stored in glMap1, displayed as a line strip
    void renderMap() const;

    // renders a covering piece of knot at the bottom-right corner
    void renderCover() const;

    // renders a straight left-to-right pipe
    void renderPipe() const;

    // renders a diagonal top-left to bottom-right pipe, covered in the bottom right
    void renderDiagPipe() const;

    // renders a left-to-bottom corner
    void renderCorner() const;

    // renders a left-to-bottom-right over bend
    void renderOverBend() const;

    // renders a left-to-bottom-right under bend
    void renderUnderBend() const;


    float cell_size_;   // the cell size
    float half_size_;   // half the cell size

    float ribbon_size_; // width of the ribbon in the cell
    float border_size_; // width of the border on either side of the ribbon

    lemon::Array<float, 3> ribbon_color_;
    lemon::Array<float, 3> border_color_;
};

#endif
