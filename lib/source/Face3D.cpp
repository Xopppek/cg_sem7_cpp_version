#include "Face3D.h"

#include <algorithm>
#include <stdexcept>

Face3D::Face3D() {
    this->vertices = std::vector<Vector3D>(3);
}

Face3D::Face3D(const std::vector<Vector3D> &vertices) {
    if (vertices.size() < 3) {
        throw std::invalid_argument("Not enough vertices provided");
    }
    this->vertices = vertices;
}

int Face3D::getVerticesCount() const {
    return static_cast<int>(vertices.size());
}

Vector3D Face3D::getNormal() const {
    Vector3D v1 = vertices[1] - vertices[0];
    Vector3D v2 = vertices[2] - vertices[0];
    return v1.cross(v2);
}

std::vector<Vector3D> Face3D::getVertices() const {
    return this->vertices;
}

Face3D Face3D::applyTransformation(const std::vector<std::vector<double>>& T) const {
    std::vector<Vector3D> transformedVertices(getVerticesCount());
    std::ranges::transform(vertices, transformedVertices.begin(),
                           [&](const Vector3D& v) {return v.applyTransformation(T);});
    return {transformedVertices};
}

Face3D Face3D::rotateAroundAxis(double angle, const Vector3D &axis) const {
    std::vector<Vector3D> rotatedVertices(getVerticesCount());
    std::ranges::transform(vertices, rotatedVertices.begin(),
                           [&](const Vector3D& v) {return v.rotateAroundAxis(angle, axis);});
    return {rotatedVertices};
}

Polygon Face3D::getParallelProjectionOnXY() const {
    std::vector<Vector2D> parallelProjection(getVerticesCount());
    std::ranges::transform(vertices, parallelProjection.begin(),
                           [](const Vector3D& v) {return v.getParallelProjectionOnXY();});
    return Polygon(parallelProjection);
}

Face3D Face3D::onePointOnZPerspectiveTransformation(double r) const {
    std::vector<Vector3D> transformedVertices(getVerticesCount());
    std::ranges::transform(vertices, transformedVertices.begin(),
                           [&](const Vector3D& v) {return v.onePointOnZPerspectiveTransformation(r);});
    return {transformedVertices};
}

