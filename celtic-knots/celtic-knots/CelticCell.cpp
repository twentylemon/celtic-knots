

#include "CelticCell.h"


CelticCell::CelticCell() :
    x_(-1),
    y_(-1),
    up_(false),
    down_(false),
    left_(false),
    right_(false)
{
}

CelticCell::CelticCell(int x, int y) :
    x_(x),
    y_(y),
    up_(false),
    down_(false),
    left_(false),
    right_(false)
{
}


bool CelticCell::operator==(const CelticCell& rhs) const { return x() == rhs.x() && y() == rhs.y(); }

int CelticCell::x() const {
    return x_;
}

int CelticCell::y() const {
    return y_;
}

bool CelticCell::up() const {
    return up_;
}

bool CelticCell::down() const {
    return down_;
}

bool CelticCell::left() const {
    return left_;
}

bool CelticCell::right() const {
    return right_;
}

int CelticCell::ord() const {
    int o = 0;
    if (up()) { o++; }
    if (down()) { o++; }
    if (left()) { o++; }
    if (right()) { o++; }
    return o;
}

void CelticCell::set_x(int x) {
    x_ = x;
}

void CelticCell::set_y(int y) {
    y_ = y;
}

void CelticCell::set_up(bool up) {
    up_ = up;
}

void CelticCell::set_down(bool down) {
    down_ = down;
}

void CelticCell::set_left(bool left) {
    left_ = left;
}

void CelticCell::set_right(bool right) {
    right_ = right;
}
