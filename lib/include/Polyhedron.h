#ifndef POLYHEDRON_H
#define POLYHEDRON_H
#include "Face3D.h"

class Polyhedron {
public:
    Polyhedron(const std::vector<Face3D>& faces);

    [[nodiscard]] int getFacesCount() const;
    [[nodiscard]] std::vector<Face3D> getFaces() const;

    [[nodiscard]] Polyhedron applyTransformation(const std::vector<std::vector<double>>& T) const;
    [[nodiscard]] Polyhedron rotateAroundAxis(double angle, const Vector3D& axis) const;

    [[nodiscard]] std::vector<Polygon> getParallelProjectionOnXY() const;
    [[nodiscard]] std::vector<Polygon> getParallelProjectionOnXY(const Vector3D& viewer) const;
    [[nodiscard]] std::vector<Polygon> getPerspectiveOnePointOnZProjectionOnXZ(double r) const;
    [[nodiscard]] std::vector<Polygon> getPerspectiveOnePointOnZProjectionOnXZ(double r, const Vector3D& viewer) const;
private:
    std::vector<Face3D> faces;
};

#endif //POLYHEDRON_H
