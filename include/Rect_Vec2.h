#ifndef RECT_VEC2_H
#define RECT_VEC2_H

#include <cmath>

class Vec2
{
public:
    float x, y;

    Vec2(float x = 0, float y = 0);

    Vec2 operator+(const Vec2 &other) const;
    Vec2 operator-(const Vec2 &other) const;
    Vec2 operator*(float scalar) const;
    float Magnitude() const;
    Vec2 Normalize() const;
    float Distance(const Vec2 &other) const;
    float Angle() const;
    Vec2 Rotate(float angle) const;
};

class Rect
{
public:
    float x, y, w, h;

    Rect(float x = 0, float y = 0, float w = 0, float h = 0);

    Vec2 GetCenter() const;
    float Distance(const Rect &other) const;
    bool Contains(float px, float py) const;
    Rect operator+(const Vec2 &vec) const;
    void SetCenter(Vec2 center);
};

#endif // RECT_VEC2_H
