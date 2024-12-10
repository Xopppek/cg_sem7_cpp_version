#include "Vector3D.h"

#include <complex>
#include <sstream>

Vector3D::Vector3D() {x = 0; y = 0; z = 0;}

Vector3D::Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

double Vector3D::getX() const { return x; }
double Vector3D::getY() const { return y; }
double Vector3D::getZ() const { return z; }

Vector3D Vector3D::operator+(const Vector3D& other) const {
    return {x + other.x, y + other.y, z + other.z};
}

Vector3D Vector3D::operator-(const Vector3D& other) const {
    return {x - other.x, y - other.y, z - other.z};
}

Vector3D Vector3D::operator*(double scalar) const {
    return {scalar * x, scalar * y, scalar * z};
}

Vector3D& Vector3D::operator+=(const Vector3D& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

bool Vector3D::operator==(const Vector3D& other) const {
    return (x - other.x < 1e-9) && (y - other.y < 1e-9) && (z - other.z < 1e-9);
}

bool Vector3D::operator!=(const Vector3D &other) const {
    return !(*this == other);
}

Vector3D operator*(double scalar, const Vector3D& vector) {
    return {scalar * vector.x, scalar * vector.y, scalar * vector.z};
}

std::string Vector3D::toString() const {
    std::stringstream ss;
    ss << "[" << x << ", " << y << ", " << z << "]";
    return ss.str();
}

double Vector3D::dot(const Vector3D& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector3D Vector3D::cross(const Vector3D& other) const {
    return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
}

Vector3D Vector3D::applyTransformation(const std::vector<std::vector<double>>& T) const {
    if (T.size() != 4 || T[0].size() != 4 || T[1].size() != 4 || T[2].size() != 4 || T[3].size() != 4)
        throw std::invalid_argument("Vector3D::apply(): wrong size of transfromation matrix");

    std::vector<double> homogeneousCoordinates = {getX(), getY(), getZ(), 1};
    double X = 0, Y = 0, Z = 0, H = 0;
    for (int i = 0; i < 4; i++) {
        X += T[i][0] * homogeneousCoordinates[i];
        Y += T[i][1] * homogeneousCoordinates[i];
        Z += T[i][2] * homogeneousCoordinates[i];
        H += T[i][3] * homogeneousCoordinates[i];
    }

    return {X / H, Y / H, Z / H};
}

Vector3D Vector3D::rotateAroundAxis(double angle, const Vector3D& axis) const {
    if (axis.dot(axis) == 0)
        throw std::invalid_argument("Vector3D::rotateAroundAxis(): axis is zero");

    double axisNorm = sqrt(axis.dot(axis));
    double nx = axis.getX() / axisNorm, ny = axis.getY() / axisNorm, nz = axis.getZ() / axisNorm;

    std::vector<std::vector<double>> T =
        {
            {
                cos(angle) + nx * nx * (1 - cos(angle)),
                nx * ny * (1 - cos(angle)) + nz * sin(angle),
                nx * nz * (1 - cos(angle)) - ny * sin(angle),
                0
            },
            {
                nx * ny * (1 - cos(angle)) - nz * sin(angle),
                cos(angle) + ny * ny * (1 - cos(angle)),
                ny * nz * (1 - cos(angle)) + nx * sin(angle),
                0
            },
            {
                nx * nz * (1 - cos(angle)) + ny * sin(angle),
                ny * nz * (1 - cos(angle)) - nx * sin(angle),
                cos(angle) + nz * nz * (1 - cos(angle)),
                0
            },
            {0, 0, 0, 1}
        };

    return applyTransformation(T);
}

Vector2D Vector3D::getParallelProjectionOnXY() const {
    return {getX(), getY()};
}

Vector3D Vector3D::onePointOnZPerspectiveTransformation(double r) const {
    // std::vector<std::vector<double>> T = {
    //     {1, 0, 0, 0},
    //     {0, 1, 0, 0},
    //     {0, 0, 1, r},
    //     {0, 0, 0, 1},
    // };
    // return applyTransformation(T);
    return {x / (r * z + 1), y / (r * z + 1), z / (r * z + 1)};
}

