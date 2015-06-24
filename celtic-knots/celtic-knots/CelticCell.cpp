

#include "CelticCell.h"


CelticCell::CelticCell() :
    up(false),
    down(false),
    left(false),
    right(false)
{
}

CelticCell::CelticCell(bool up, bool down, bool left, bool right) :
    up(up),
    down(down),
    left(left),
    right(right)
{
}

bool CelticCell::operator==(const CelticCell& rhs) const {
    return false;
}
