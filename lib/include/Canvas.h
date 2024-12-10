#ifndef CANVAS_H
#define CANVAS_H

#include <opencv2/opencv.hpp>
#include "Vector2D.h"
#include "Polygon.h"

class Canvas {
public:
    Canvas(int width, int height);

    Canvas drawPoint(const Vector2D& point, const cv::Vec3b& color);
    Canvas drawLine(const Vector2D& start, const Vector2D& end, const cv::Vec3b& color);
    Canvas drawPolygon(const Polygon& polygon, const cv::Vec3b& color);
    Canvas fillPolygonEO(const Polygon& polygon, const cv::Vec3b& color);
    Canvas fillPolygonNZW(const Polygon& polygon, const cv::Vec3b& color);
    Canvas drawBezierCubic(const Vector2D& p0, const Vector2D& p1,
                           const Vector2D& p2, const Vector2D& p3, const cv::Vec3b& color);

    [[nodiscard]] cv::Mat getImage() const;
    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;

private:
    int width, height;
    cv::Mat image;
};

#endif //CANVAS_H
