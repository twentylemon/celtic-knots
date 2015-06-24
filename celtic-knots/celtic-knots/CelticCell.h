
#ifndef CELTICCELL_H
#define CELTICCELL_H

class CelticCell
{
public:
    // sets all members to false
    CelticCell();

    // sets the position of this cell, all edges around to false
    CelticCell(int x, int y);

    // for lemon::collections
    bool operator==(const CelticCell& rhs) const { return x() == rhs.x() && y() == rhs.y(); }

    int x() const { return x_; }
    int y() const { return y_; }

    bool up() const { return up_; }
    bool down() const { return down_; }
    bool left() const { return left_; }
    bool right() const { return right_; }

    void set_x(int x) { x_ = x; }
    void set_y(int y) { y_ = y; }
    void set_up(bool up) { up_ = up; }
    void set_down(bool down) { down_ = down; }
    void set_left(bool left) { left_ = left; }
    void set_right(bool right) { right_ = right; }

private:
    int x_, y_;
    bool left_, right_, up_, down_;
};

#endif
