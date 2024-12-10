#include "Vector2D.h"

#include <sstream>
#include <string>

Vector2D::Vector2D() {x = 0; y = 0;}

Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

double Vector2D::getX() const { return x; }
double Vector2D::getY() const { return y; }

Vector2D Vector2D::operator+(const Vector2D& other) const {
    return {x + other.x, y + other.y};
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
    return {x - other.x, y - other.y};
}

Vector2D Vector2D::operator*(double scalar) const {
    return {scalar * x, scalar * y};
}

Vector2D& Vector2D::operator+=(const Vector2D& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

bool Vector2D::operator==(const Vector2D& other) const {
    return (x - other.x < 1e-9) && (y - other.y < 1e-9);
}

bool Vector2D::operator!=(const Vector2D &other) const {
    return !(*this == other);
}

Vector2D operator*(double scalar, const Vector2D& vector) {
    return {scalar * vector.x, scalar * vector.y};
}

std::string Vector2D::toString() const {
    std::stringstream ss;
    ss << "[" << x << ", " << y << "]";
    return ss.str();
}