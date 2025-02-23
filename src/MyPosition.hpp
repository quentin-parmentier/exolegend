#ifndef MYPOSITION_HPP_
#define MYPOSITION_HPP_

#include "Vector2.hpp"

class MyPosition
{
public:
    MyPosition();
    MyPosition(int x, int y);
    Vector2 toVector(double cellSize) const;
    int getX() const;
    int getY() const;
    void copy(MyPosition p);

    bool operator<(const MyPosition& other) const {
        // Comparaison par x, puis y si les x sont égaux
        if (_x != other._x) {
            return _x < other._x;
        }
        return _y < other._y;
    }

private:
    int _x, _y;
};

#endif