#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <string>
#include <vector>
#include "Vector2D.h"

class Vector3D {
public:
    Vector3D();
    Vector3D(double x, double y, double z);

    [[nodiscard]] double getX() const;
    [[nodiscard]] double getY() const;
    [[nodiscard]] double getZ() const;

    Vector3D operator+(const Vector3D &other) const;
    Vector3D operator-(const Vector3D &other) const;
    Vector3D operator*(double scalar) const;
    Vector3D& operator+=(const Vector3D &other);
    Vector3D& operator-=(const Vector3D &other);
    friend Vector3D operator*(double scalar, const Vector3D &vector);
    bool operator==(const Vector3D &other) const;
    bool operator!=(const Vector3D &other) const;

    [[nodiscard]] std::string toString() const;

    [[nodiscard]] double dot(const Vector3D &other) const;
    [[nodiscard]] Vector3D cross(const Vector3D &other) const;
    [[nodiscard]] Vector3D applyTransformation(const std::vector<std::vector<double>>& T) const;
    [[nodiscard]] Vector3D rotateAroundAxis(double angle, const Vector3D& axis) const;

    [[nodiscard]] Vector3D onePointOnZPerspectiveTransformation(double r) const;
    [[nodiscard]] Vector2D getParallelProjectionOnXY() const;

private:
    double x, y, z;
};

#endif //VECTOR3D_H
