
#ifndef BREAKMARKER_H
#define BREAKMARKER_H

#include <iostream>

class BreakMarker
{
public:

    static enum Direction { East = 'E', South = 'S' };

    // initializes the break marker
    BreakMarker(int x, int y, Direction direction);
    
    // returns the x coord of the top-left of this marker
    int& x();
    
    // returns the x coord of the top-left of this marker
    const int& x() const;
    
    // returns the y coord of the top-left of this marker
    int& y();
    
    // returns the y coord of the top-left of this marker
    const int& y() const;
    
    // returns the direction this marker goes
    Direction& direction();
    
    // returns the direction this marker goes
    const Direction& direction() const;

    // sets the new x location
    void setX(int x);

    // sets the new y location
    void setY(int y);

    // sets the new direction
    void setDirection(Direction direction);

    // returns true if this marker is < rhs by tuple comparison (x,y,dir)
    bool operator<(const BreakMarker& rhs) const;

    // returns true if this marker is tuple == to rhs
    bool operator==(const BreakMarker& rhs) const;

    // writes this marker to the stream, excluding std::endl
    friend std::ostream& operator<<(std::ostream& out, const BreakMarker& marker);

private:
    int x_, y_;             // top-left position of this marker
    Direction direction_;   // the direction this marker goes
};

#endif
