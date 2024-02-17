#include "MyPosition.hpp"

MyPosition::MyPosition() : _x(0), _y(0) {}
MyPosition::MyPosition(int x, int y) : _x(x), _y(y) {}

Vector2 MyPosition::toVector(double cellSize) const { return Vector2(_x * cellSize + 0.5 * cellSize, _y * cellSize + 0.5 * cellSize); }

int MyPosition::getX() const { return _x; }

int MyPosition::getY() const { return _y; }

void MyPosition::copy(MyPosition p)
{
    _x = p.getX();
    _y = p.getY();
}
