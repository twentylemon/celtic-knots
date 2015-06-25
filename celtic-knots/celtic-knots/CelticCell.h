
#ifndef CELTICCELL_H
#define CELTICCELL_H

// one cell in the grid, stores position and flags indicating neighbouring break markers
class CelticCell
{
public:
    // sets all members to false
    CelticCell();

    // sets the position of this cell, all edges around to false
    CelticCell(int x, int y);

    // returns true if this and rhs are equal
    bool operator==(const CelticCell& rhs) const;

    // returns the x position in the grid
    int x() const;

    // returns the y position in the grid
    int y() const;

    // returns true if there is a marker edge above this cell
    bool up() const;

    // returns true if there is a marker edge below this cell
    bool down() const;

    // returns true if there is a marker edge to the left of this cell
    bool left() const;

    // returns true if there is a marker edge to the right of this cell
    bool right() const;

    // returns the number of break markers around this cell
    int ord() const;

    // sets the x position
    void set_x(int x);

    // sets the new y position
    void set_y(int y);

    // sets if there a marker edge above this cell
    void set_up(bool up);

    // sets if there a marker edge below this cell
    void set_down(bool down);

    // sets if there a marker edge to the left of this cell
    void set_left(bool left);

    // sets if there a marker edge to the right of this cell
    void set_right(bool right);

private:
    int x_, y_;
    bool left_, right_, up_, down_;
};

#endif
