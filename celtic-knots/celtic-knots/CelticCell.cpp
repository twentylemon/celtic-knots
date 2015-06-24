

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
