#ifndef FACE3D_H
#define FACE3D_H

#include <vector>

#include "Vector3D.h"
#include "Polygon.h"

class Face3D {
public:
    Face3D();
    Face3D(const std::vector<Vector3D>& vertices);

    [[nodiscard]] int getVerticesCount() const;
    [[nodiscard]] std::vector<Vector3D> getVertices() const;
    [[nodiscard]] Vector3D getNormal() const;

    [[nodiscard]] Face3D applyTransformation(const std::vector<std::vector<double>>& T) const;
    [[nodiscard]] Face3D rotateAroundAxis(double angle, const Vector3D& axis) const;

    [[nodiscard]] Face3D onePointOnZPerspectiveTransformation(double r) const;
    [[nodiscard]] Polygon getParallelProjectionOnXY() const;

private:
    std::vector<Vector3D> vertices;
};

#endif //FACE3D_H
