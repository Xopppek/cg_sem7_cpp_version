#include "Polygon.h"

#include <stdexcept>
#include <sstream>
#include <vector>

Polygon::Polygon() {
    this->vertexes = std::vector<Vector2D>();
}

Polygon::Polygon(std::vector<Vector2D> vertexes) {
    this->vertexes = std::move(vertexes);
}

Vector2D Polygon::getVertex(int index) const {
    if (index >= vertexes.size() || index < 0) {
        throw std::out_of_range("index out of range");
    }
    return vertexes[index];
}

int Polygon::getVertexCount() const {
    return static_cast<int>(vertexes.size());
}

std::string Polygon::toString() const {
    std::stringstream ss;
    ss << "{ ";
    for (auto vertex : this->vertexes) {
        ss << vertex.toString() << ", ";
    }
    ss << "\b\b }";
    return ss.str();
}

enum CLPointType {
    LEFT,
    RIGHT,
    BEYOND,
    BEHIND,
    BETWEEN,
    ORIGIN,
    DESTINATION,
};

CLPointType Classify(const Vector2D& v1, const Vector2D& v2, const Vector2D& v) {
    double ax = v2.getX() - v1.getX();
    double ay = v2.getY() - v1.getY();
    double bx = v.getX() - v1.getX();
    double by = v.getY() - v1.getY();
    double s = ax * by - bx * ay;
    if (s > 0) return LEFT;
    if (s < 0) return RIGHT;
    if ((ax*bx < 0) || (ay*by < 0)) //противоположно направлению
        return BEHIND; //позади
    if ((ax*ax + ay*ay) < (bx*bx + by*by))
        return BEHIND; //впереди
    if (v1 == v) //совпадает с началом
        return ORIGIN;
    if (v2 == v) //совпадает с концом
        return DESTINATION;
    return BETWEEN; //между
}

enum IntersectType {
    SAME,
    PARALLEL,
    SKEW,
    SKEW_CROSS,
    SKEW_NO_CROSS,
};

IntersectType Intersect(Vector2D a, Vector2D b, Vector2D c, Vector2D d, double *t) {
    double ax = a.getX(), ay = a.getY();
    double bx = b.getX(), by = b.getY();
    double cx = c.getX(), cy = c.getY();
    double dx = d.getX(), dy = d.getY();
    double nx= dy - cy; //вычисление координат n
    double ny= cx - dx;
    CLPointType type;
    double denom= nx*(bx - ax) + ny*(by - ay); //n ∗ b − a
    if (denom == 0) { //параллельны или совпадают
        type = Classify(c, d, a); // положение точки a относительно прямой cd
        if (type == LEFT || type == RIGHT)
            return PARALLEL;
        return SAME;
    }
    double num= nx*(ax - cx) + ny*(ay - cy); //n ∗ a − c
    *t= -num/denom; // по значению t можно сделать вывод о пересечении отрезка ab
    return SKEW;
}

IntersectType Cross(Vector2D a, Vector2D b, Vector2D c, Vector2D d, double *tab, double *tcd) {
    IntersectType type= Intersect(a, b, c, d, tab);
    if (type==SAME || type==PARALLEL)
        return type;
    if ((*tab < 0) || (*tab > 1))
        return SKEW_NO_CROSS;
    Intersect(c, d, a, b, tcd);
    if ((*tcd < 0) || (*tcd > 1))
        return SKEW_NO_CROSS;
    return SKEW_CROSS;
}

enum EType {
    TOUCHING,
    CROSS_LEFT,
    CROSS_RIGHT,
    INESSENTIAL,
};

EType EdgeType(Vector2D o, Vector2D d, Vector2D a) {
    switch(Classify(o, d, a)) {
        case LEFT:
            if(a.getY() > o.getY() && a.getY() <= d.getY()) return CROSS_LEFT; // пересекающая, A слева
            else return INESSENTIAL; // безразличная
        case RIGHT:
            if(a.getY() > d.getY() && a.getY() <= o.getY()) return CROSS_RIGHT; // пересекающая, A справа
            else return INESSENTIAL; // безразличная
        case BETWEEN:
        case ORIGIN:
        case DESTINATION:
        return TOUCHING; // касающаяся
        default:
            return INESSENTIAL; // безразличная
    }
}

bool Polygon::isPointInsideEO(const Vector2D &point) const {
    const int n = getVertexCount();
    bool param = false;

    for (int i = 0; i < n; ++i) {
        switch (EdgeType(getVertex(i), getVertex((i + 1) % n), point)) {
            case TOUCHING:
                return true;
            case CROSS_LEFT:
            case CROSS_RIGHT:
                param = !param;
            default: ;
        }
    }
    return param;
}

bool Polygon::isPointInsideNZW(const Vector2D &point) const {
    const int n = getVertexCount();
    int param = 0;

    for (int i = 0; i < n; ++i) {
        switch (EdgeType(getVertex(i), getVertex((i + 1) % n), point)) {
            case TOUCHING:
                return true;
            case CROSS_LEFT:
                param++;
                break;
            case CROSS_RIGHT:
                param--;
                break;;
            default: ;
        }
    }
    return param != 0;
}

bool Polygon::hasSelfIntersection() const {
    int n = getVertexCount();
    if (n < 4)
        return false;

    for (int i = 0; i < n; i++){
        Vector2D a = getVertex(i);
        Vector2D b = getVertex((i + 1) % n);

        for (int j = i + 2; j < n; j++){
            if (i == 0 && j == n - 1)
                continue;
            Vector2D c = getVertex(j);
            Vector2D d = getVertex((j + 1) % n);
            double tab, tcd;
            IntersectType intersectType = Cross(a, b, c, d, &tab, &tcd);
            if (intersectType == SKEW_CROSS)
                return true;
        }
    }
    return false;
}

bool Polygon::isConvex() const {
    int n = getVertexCount();
    if (n < 3)
        return true;

    if (hasSelfIntersection())
        return false;

    bool hasPositiveRotation = false;
    bool hasNegativeRotation = false;

    for (int i = 0; i < n; i++){
        Vector2D a = getVertex(i);
        Vector2D b = getVertex((i + 1) % n);
        Vector2D c = getVertex((i + 2) % n);
        Vector2D ab = b - a;
        Vector2D bc = c - b;
        double product = ab.getX() * bc.getY() - ab.getY() * bc.getX();
        if (product > 0)
            hasPositiveRotation = true;
        if (product < 0)
            hasNegativeRotation = true;
        if (hasNegativeRotation && hasPositiveRotation)
            return false;
    }
    return true;
}

int Polygon::CyrusBeckClipLine(Vector2D &v1, Vector2D &v2) const {
    int n = getVertexCount();
    Polygon clipper = *this;
    if (!isClockWiseOriented())
        clipper = switchOrientation();

    double t1 = 0, t2 = 1;
    Vector2D s = v2 - v1;
    for(int i = 0; i < n; i++) {
        double nx = (clipper.getVertex((i + 1) % n) - clipper.getVertex(i)).getY();
        double ny = (clipper.getVertex(i) - clipper.getVertex((i + 1) % n)).getX();; // нормаль к стороне полигона
        double denom = nx * s.getX() + ny*s.getY(); //нахождение точки пересечения
        double num = nx * (v1.getX() - clipper.getVertex(i).getX()) + ny * (v1.getY() - clipper.getVertex(i).getY());
        if(denom != 0) { //линии не параллельны
            double t = -num/denom;
            if( denom > 0) { //точка t потенциально входящая (ПВ)
                if(t > t1) t1=t; }
            else { //потенциально покидающая (ПП)

                if(t < t2) t2=t; }

        } else { if (Classify(clipper.getVertex(i), clipper.getVertex((i + 1) % n), v1)==LEFT) return 0; } // параллельны
    } //for
    if(t1 <= t2) { //существует видимый отрезок
        Vector2D v1_new = v1 + t1 * (v2 - v1);
        Vector2D v2_new = v1 + t2 * (v2 - v1);
        v1 = v1_new; v2 = v2_new;
        return 1;
    }
    return 0;
}

bool Polygon::isClockWiseOriented() const {
    int n = getVertexCount();
    bool hasPositiveRotation = false;

    for (int i = 0; i < n; i++){
        Vector2D a = getVertex(i);
        Vector2D b = getVertex((i + 1) % n);
        Vector2D c = getVertex((i + 2) % n);
        Vector2D ab = b - a;
        Vector2D bc = c - b;
        double product = ab.getX() * bc.getY() - ab.getY() * bc.getX();
        if (product > 0)
            hasPositiveRotation = true;
        if (hasPositiveRotation)
            return false;
    }
    return true;
}


Polygon Polygon::switchOrientation() const {
    int n = getVertexCount();
    std::vector<Vector2D> vertices_new(n);
    for (int i = 0; i < n; i++) {
        vertices_new[i] = getVertex(n - i - 1);
    }
    return Polygon(vertices_new);
}

std::vector<Vector2D> clipLineByLine(Vector2D p1, Vector2D p2, Vector2D a, Vector2D b) {
    double t1 = 0, t2 = 1, t;
    double sx = p2.getX() - p1.getX(), sy = p2.getY() - p1.getY();

    double nx = (b - a).getY();
    double ny = (a - b).getX();
    double denom = nx * sx + ny * sy;
    double num = nx * (p1.getX() - a.getX()) +
                 ny * (p1.getY() - a.getY());
    if (denom != 0) {
        t = -num / denom;
        if (denom > 0) {
            if (t > t1)
                t1 = t;
        }
        else {
            if (t < t2)
                t2 = t;
        }
    } else {
        if (Classify(a, b, p1) == LEFT)
            return {};
    }

    if (t1 <= t2) {
        Vector2D p1Cut = p1 + t1 * (p2 - p1);
        Vector2D p2Cut = p1 + t2 * (p2 - p1);
        if (p1 == p1Cut) {
            return {p2Cut};
        } else if (p2 == p2Cut) {
            return {p1Cut, p2Cut};
        }
    }
    return {};
}

Polygon Polygon::SutherlandHodgman(const Polygon& clipper) const {
    Polygon thisPolygon = *this;
    Polygon otherPolygon = clipper;
    if (!clipper.isClockWiseOriented())
        otherPolygon = clipper.switchOrientation();
    if (!thisPolygon.isClockWiseOriented())
        thisPolygon = thisPolygon.switchOrientation();

    std::vector<Vector2D> result = thisPolygon.vertexes;

    for (int i = 0; i < otherPolygon.getVertexCount(); i++) {
        Vector2D aCur = otherPolygon.getVertex(i);
        Vector2D bCur = otherPolygon.getVertex((i + 1) % otherPolygon.getVertexCount());
        Polygon curPolygon(result);

        result.clear();

        for (int j = 0; j < curPolygon.getVertexCount(); j++) {
            Vector2D p1 = curPolygon.getVertex(j);
            Vector2D p2 = curPolygon.getVertex((j + 1) % curPolygon.getVertexCount());
            std::vector<Vector2D> clipped = clipLineByLine(p1, p2, aCur, bCur);
            for (Vector2D v : clipped) {
                result.push_back(v);
            }
        }
    }
    return Polygon(result);
}

