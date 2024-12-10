#include "Canvas.h"

Canvas::Canvas(int width, int height) {
    this->width = width;
    this->height = height;
    this->image = cv::Mat(width, height, CV_8UC3, cv::Vec3b(0, 0, 0));
}

cv::Mat Canvas::getImage() const {
    return this->image;
}

int Canvas::getHeight() const {
    return this->height;
}

int Canvas::getWidth() const {
    return this->width;
}

Canvas Canvas::drawPoint(const Vector2D& point, const cv::Vec3b& color) {
    if (point.getX() < 0 || point.getY() < 0) {
        return *this;
    }
    if (point.getX() >= this->width || point.getY() >= this->height) {
        return *this;
    }
    int x = static_cast<int>(round(point.getX()));
    int y = static_cast<int>(round(point.getY()));
    image.at<cv::Vec3b>(y, x) = color;
    return *this;
}

Canvas Canvas::drawLine(const Vector2D &start, const Vector2D &end, const cv::Vec3b& color) {
    int x = static_cast<int>(round(start.getX()));
    int y = static_cast<int>(round(start.getY()));
    int dx = static_cast<int>(round(end.getX())) - x;
    int dy = static_cast<int>(round(end.getY()) - y);
    int e, i;
    int ix = dx > 0 ? 1 : -1;
    int iy = dy > 0 ? 1 : -1;
    dx = abs(dx);
    dy = abs(dy);
    if (dx >= dy) {
        e = 2 * dy - dx;
        if (iy >= 0) {
            for (i = 0; i <= dx; i++) {
                drawPoint(Vector2D(x, y), color);
                if (e >= 0) {
                    y += iy;
                    e -= 2 * dx;
                }
                x += ix;
                e += dy * 2;
            }
        } else {
            for (i = 0; i <= dx; i++) {
                drawPoint(Vector2D(x, y), color);
                if (e > 0) {
                    y += iy;
                    e -= 2 * dx;
                }
                x += ix;
                e += dy * 2;
            }
        }
    } else {
        e = 2 * dx - dy;
        if (ix >= 0) {
            for (i = 0; i <= dy; i++) {
                drawPoint(Vector2D(x, y), color);
                if (e >= 0) {
                    x += ix;
                    e -= 2 * dy;
                }
                y += iy;
                e += dx * 2;
            }
        } else {
            for (i = 0; i <= dy; i++) {
                drawPoint(Vector2D(x, y), color);
                if (e > 0) {
                    x += ix;
                    e -= 2 * dy;
                }
                y += iy;
                e += dx * 2;
            }
        }
    }
    return *this;
}

Canvas Canvas::drawPolygon(const Polygon &polygon, const cv::Vec3b &color) {
    const int vertexCount = polygon.getVertexCount();

    if (vertexCount == 0) {
        return *this;
    }
    if (vertexCount == 1) {
        drawPoint(polygon.getVertex(0), color);
        return *this;
    }
    if (vertexCount == 2) {
        drawLine(polygon.getVertex(0), polygon.getVertex(1), color);
        return *this;
    }

    for (int i = 1; i < vertexCount + 1; i++) {
        drawLine(polygon.getVertex(i - 1), polygon.getVertex(i % vertexCount), color);
    }
    return *this;
}

void getBorders(const Polygon &polygon, int& xMin_, int& yMin_, int& xMax_, int& yMax_) {
    double xMin = static_cast<int>(round(polygon.getVertex(0).getX()));
    double yMin = static_cast<int>(round(polygon.getVertex(0).getY()));
    double xMax = static_cast<int>(round(polygon.getVertex(1).getX()));
    double yMax = static_cast<int>(round(polygon.getVertex(1).getY()));

    for (int i = 1; i < polygon.getVertexCount(); ++i) {
        if (xMin > polygon.getVertex(i).getX())
            xMin = static_cast<int>(round(polygon.getVertex(i).getX()));
        if (yMin > polygon.getVertex(i).getY())
            yMin = static_cast<int>(round(polygon.getVertex(i).getY()));
        if (xMax < polygon.getVertex(i).getX())
            xMax = static_cast<int>(round(polygon.getVertex(i).getX()));
        if (yMax < polygon.getVertex(i).getY())
            yMax = static_cast<int>(round(polygon.getVertex(i).getY()));
    }

    xMin_ = static_cast<int>(round(xMin));
    yMin_ = static_cast<int>(round(yMin));
    xMax_ = static_cast<int>(round(xMax));
    yMax_ = static_cast<int>(round(yMax));
}

Canvas Canvas::fillPolygonEO(const Polygon &polygon, const cv::Vec3b &color) {
    int xMin, yMin, xMax, yMax;
    getBorders(polygon, xMin, yMin, xMax, yMax);

    for (int x = xMin; x <= xMax; x++) {
        for (int y = yMin; y <= yMax; y++) {
            if (polygon.isPointInsideEO(Vector2D(x, y)))
                drawPoint(Vector2D(x, y), color);
        }
    }
    return *this;
}

Canvas Canvas::fillPolygonNZW(const Polygon &polygon, const cv::Vec3b &color) {
    int xMin, yMin, xMax, yMax;
    getBorders(polygon, xMin, yMin, xMax, yMax);

    for (int x = xMin; x <= xMax; x++) {
        for (int y = yMin; y <= yMax; y++) {
            if (polygon.isPointInsideNZW(Vector2D(x, y)))
                drawPoint(Vector2D(x, y), color);
        }
    }
    return *this;
}

Vector2D line(Vector2D p0, Vector2D p1, double t) {
    return p0 * (1 - t) + p1 * t;
}

Vector2D BezierQuadratic(Vector2D p0, Vector2D p1, Vector2D p2, double t) {
    return line(line(p0, p1, t), line(p1, p2, t), t);
}

Vector2D BezierCubic(Vector2D p0, Vector2D p1, Vector2D p2, Vector2D p3, double t) {
    return line(BezierQuadratic(p0, p1, p2, t), BezierQuadratic(p1, p2, p3, t), t);
}

double Dist(const Vector2D& vector) {
    return abs(vector.getX()) + abs(vector.getY());
}

Canvas Canvas::drawBezierCubic(const Vector2D& p0, const Vector2D& p1,
                               const Vector2D& p2, const Vector2D& p3, const cv::Vec3b &color) {
    double H = cv::max(Dist(p0 - 2 * p1 + p2), Dist(p1 - 2 * p2 + p3));
    double N = 1 + sqrt(3 * H);
    Vector2D left = p0;
    Vector2D right = p0;
    for (double t = 0; t < 1; t += 1 / N) {
        right = BezierCubic(p0, p1, p2, p3, t);
        drawLine(left, right, color);
        left = right;
    }
    drawLine(left, p3, color);
    return *this;
}

