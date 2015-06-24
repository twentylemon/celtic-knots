

#include "BreakMarker.h"

BreakMarker::BreakMarker(int x, int y, Direction direction) :
    x_(x),
    y_(y),
    direction_(direction)
{
}
    

int& BreakMarker::x() {
    return x_;
}

const int& BreakMarker::x() const {
    return x_;
}

int& BreakMarker::y() {
    return y_;
}

const int& BreakMarker::y() const {
    return y_;
}

BreakMarker::Direction& BreakMarker::direction() {
    return direction_;
}

const BreakMarker::Direction& BreakMarker::direction() const {
    return direction_;
}

void BreakMarker::setX(int x) {
    x_ = x;
}

void BreakMarker::setY(int y) {
    y_ = y;
}

void BreakMarker::setDirection(Direction direction) {
    direction_ = direction;
}

bool BreakMarker::operator<(const BreakMarker& rhs) const {
    if (x() < rhs.x()) {
        return true;
    }
    else if (x() == rhs.x() && y() < rhs.y()) {
        return true;
    }
    else if (x() == rhs.x() && y() == rhs.y() && direction() < rhs.direction()) {
        return true;
    }
    return false;
}

bool BreakMarker::operator==(const BreakMarker& rhs) const {
    return x() == rhs.x() && y() == rhs.y() && direction() == rhs.direction();
}


std::ostream& operator<<(std::ostream& out, const BreakMarker& marker) {
    out << marker.x() << "\t" << marker.y() << "\t" << (char)marker.direction();
    return out;
}