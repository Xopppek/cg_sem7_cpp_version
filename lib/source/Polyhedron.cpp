#include "Polyhedron.h"

#include <algorithm>

Polyhedron::Polyhedron(const std::vector<Face3D> &faces) {
    this->faces = faces;
}

int Polyhedron::getFacesCount() const {
    return static_cast<int>(faces.size());
}


std::vector<Face3D> Polyhedron::getFaces() const {
    return this->faces;
}

Polyhedron Polyhedron::applyTransformation(const std::vector<std::vector<double> > &T) const {
    std::vector<Face3D> transformedFaces(getFacesCount());
    std::ranges::transform(faces, transformedFaces.begin(),
                           [&](const Face3D& f) {return f.applyTransformation(T);});
    return {transformedFaces};
}

Polyhedron Polyhedron::rotateAroundAxis(double angle, const Vector3D& axis) const {
    std::vector<Face3D> rotatedFaces(getFacesCount());
    std::ranges::transform(faces, rotatedFaces.begin(),
                           [&](const Face3D& f) {return f.rotateAroundAxis(angle, axis);});
    return {rotatedFaces};
}

std::vector<Polygon> Polyhedron::getParallelProjectionOnXY() const {
    std::vector<Polygon> parallelProjection(getFacesCount());
    std::ranges::transform(faces, parallelProjection.begin(),
                           [&](const Face3D& f) {return f.getParallelProjectionOnXY();});
    return parallelProjection;
}

std::vector<Polygon> Polyhedron::getParallelProjectionOnXY(const Vector3D& viewer) const {
    std::vector<Polygon> parallelProjection;
    for (const auto& f : faces)
        if (f.getNormal().dot(viewer) > 0)
            parallelProjection.push_back(f.getParallelProjectionOnXY());
    return parallelProjection;
}

std::vector<Polygon> Polyhedron::getPerspectiveOnePointOnZProjectionOnXZ(double r) const {
    std::vector<Polygon> perspectiveProjection(getFacesCount());
    std::ranges::transform(faces, perspectiveProjection.begin(),
                           [&](const Face3D& f)
                           {return f.onePointOnZPerspectiveTransformation(r).getParallelProjectionOnXY();});
    return perspectiveProjection;
}

std::vector<Polygon> Polyhedron::getPerspectiveOnePointOnZProjectionOnXZ(double r, const Vector3D &viewer) const {
    std::vector<Polygon> perspectiveProjection;
    // for (auto f : faces) {
    //     f = f.onePointOnZPerspectiveTransformation(r);
    //     if (f.getNormal().dot(viewer) > 0)
    //         perspectiveProjection.push_back(f.getParallelProjectionOnXY());
    // }
    for (const auto& f : faces) {
        if (f.onePointOnZPerspectiveTransformation(r).getNormal().dot(viewer) > 0)
            perspectiveProjection.push_back(f.onePointOnZPerspectiveTransformation(r).getParallelProjectionOnXY());
    }
    return perspectiveProjection;
}

