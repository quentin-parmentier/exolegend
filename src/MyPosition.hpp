#ifndef MYPOSITION_HPP_
#define MYPOSITION_HPP_

#include "Vector2.hpp"

#define CELL_SIZE (3.0 / 12)

class MyPosition
{
public:
    MyPosition(int x, int y);
    Vector2 toVector() const;
    int getX() const;
    int getY() const;

private:
    int _x, _y;
};


#endif