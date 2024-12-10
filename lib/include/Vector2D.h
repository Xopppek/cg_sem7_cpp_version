#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <string>

class Vector2D {
public:
    Vector2D();
    Vector2D(double x, double y);

    [[nodiscard]] double getX() const;
    [[nodiscard]] double getY() const;

    Vector2D operator+(const Vector2D &other) const;
    Vector2D operator-(const Vector2D &other) const;
    Vector2D operator*(double scalar) const;
    Vector2D& operator+=(const Vector2D &other);
    Vector2D& operator-=(const Vector2D &other);
    friend Vector2D operator*(double scalar, const Vector2D &vector);
    bool operator==(const Vector2D &other) const;
    bool operator!=(const Vector2D &other) const;

    [[nodiscard]] std::string toString() const;

private:
    double x, y;
};

#endif //VECTOR2D_H
