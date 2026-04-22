#include "../include/Rect_Vec2.h"

// Implementação de Vec2
Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2 Vec2::operator+(const Vec2 &other) const { return Vec2(x + other.x, y + other.y); }
Vec2 Vec2::operator-(const Vec2 &other) const { return Vec2(x - other.x, y - other.y); }
Vec2 Vec2::operator*(float scalar) const { return Vec2(x * scalar, y * scalar); }

float Vec2::Magnitude() const { return std::sqrt(x * x + y * y); }

Vec2 Vec2::Normalize() const
{
    float mag = Magnitude();
    return (mag > 0) ? Vec2(x / mag, y / mag) : Vec2();
}

float Vec2::Distance(const Vec2 &other) const { return (*this - other).Magnitude(); }

float Vec2::Angle() const { return std::atan2(y, x); }

Vec2 Vec2::Rotate(float angle) const
{
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    return Vec2(x * cosA - y * sinA, y * cosA + x * sinA);
}

// Implementação de Rect
Rect::Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

Vec2 Rect::GetCenter() const { return Vec2(x + w / 2, y + h / 2); }

float Rect::Distance(const Rect &other) const { return GetCenter().Distance(other.GetCenter()); }

bool Rect::Contains(float px, float py) const { return px >= x && px <= x + w && py >= y && py <= y + h; }

Rect Rect::operator+(const Vec2 &vec) const { return Rect(x + vec.x, y + vec.y, w, h); }

void Rect::SetCenter(Vec2 center)
{
    x = center.x - w / 2;
    y = center.y - h / 2;
}
