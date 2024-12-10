#ifndef POLYGON_H
#define POLYGON_H

#include <string>
#include <vector>

#include "Vector2D.h"

class Polygon {
public:
    Polygon();
    explicit Polygon(std::vector<Vector2D> vertexes);

    [[nodiscard]] int getVertexCount() const;
    [[nodiscard]] Vector2D getVertex(int index) const;
    [[nodiscard]] bool isConvex() const;
    [[nodiscard]] bool hasSelfIntersection() const;
    [[nodiscard]] bool isPointInsideEO(const Vector2D& point) const;
    [[nodiscard]] bool isPointInsideNZW(const Vector2D& point) const;
    [[nodiscard]] bool isClockWiseOriented() const;
    int CyrusBeckClipLine(Vector2D& v1, Vector2D& v2) const;

    [[nodiscard]] std::string toString() const;

    [[nodiscard]] Polygon switchOrientation() const;
    [[nodiscard]] Polygon SutherlandHodgman(const Polygon& clipper) const;

private:
    std::vector<Vector2D> vertexes;
};
#endif //POLYGON_H
