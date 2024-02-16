#include "Vector2.hpp"

Vector2::Vector2() : _x(0.), _y(0.) {}

Vector2::Vector2(float x, float y) : _x(x), _y(y) {}

float Vector2::norm1() const { return std::abs(_x) + std::abs(_y); }
float Vector2::norm2() const { return std::sqrt(_x * _x + _y * _y); }
void Vector2::normalize()
{
    _x /= norm2();
    _y /= norm2();
}
Vector2 Vector2::normalized() const
{
    Vector2 out = *this;
    out.normalize();
    return out;
}

Vector2 Vector2::operator-(const Vector2 &other) const { return {_x - other._x, _y - other._y}; }
Vector2 Vector2::operator+(const Vector2 &other) const { return {_x + other._x, _y + other._y}; }
Vector2 Vector2::operator*(float f) const { return {_x * f, _y * f}; }

bool Vector2::operator==(const Vector2 &other) const { return std::abs(_x - other._x) < 1e-5 && std::abs(_y - other._y) < 1e-5; }
bool Vector2::operator!=(const Vector2 &other) const { return !(*this == other); }

float Vector2::x() const { return _x; }
float Vector2::y() const { return _y; }

float Vector2::dot(const Vector2 &other) const { return _x * other._x + _y * other._y; }
float Vector2::cross(const Vector2 &other) const { return _x * other._y - _y * other._x; }
float Vector2::angle(const Vector2 &m) const { return std::atan2(cross(m), dot(m)); }
float Vector2::angle() const { return std::atan2(_y, _x); }