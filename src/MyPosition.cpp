#include "MyPosition.hpp"

MyPosition::MyPosition() : _x(0), _y(0) {}
MyPosition::MyPosition(int x, int y) : _x(x), _y(y) {}

Vector2 MyPosition::toVector() const { return Vector2(_x * CELL_SIZE + 0.5 * CELL_SIZE, _y * CELL_SIZE + 0.5 * CELL_SIZE); }

int MyPosition::getX() const { return _x; }

int MyPosition::getY() const { return _y; }
