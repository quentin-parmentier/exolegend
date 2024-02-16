#ifndef VECTOR2_HPP_
#define VECTOR2_HPP_

#include <cmath>
#undef abs

class Vector2 {
    public:
        Vector2();
        Vector2(float x, float y);

        float norm1() const;
        float norm2() const;
        void normalize();
        Vector2 normalized() const;

        Vector2 operator-(const Vector2 &other) const;
        Vector2 operator+(const Vector2 &other) const;
        Vector2 operator*(float f) const;

        bool operator==(const Vector2 &other) const;
        bool operator!=(const Vector2 &other) const;

        float x() const;
        float y() const;

        float dot(const Vector2 &other) const;
        float cross(const Vector2 &other) const;
        float angle(const Vector2 &m) const;
        float angle() const;

    private:
        float _x, _y;
};

#endif