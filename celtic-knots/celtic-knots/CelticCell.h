
#ifndef CELTICCELL_H
#define CELTICCELL_H


class CelticCell
{
public:
    // sets all members to false
    CelticCell();
    
    // sets on to false, others to given
    CelticCell(bool up, bool down, bool left, bool right);

    bool operator==(const CelticCell& rhs) const;

    bool left, right, up, down;
};

#endif
