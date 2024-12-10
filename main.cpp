#include <iostream>
#include <opencv2/opencv.hpp>
#include "Polygon.h"
#include "Canvas.h"
#include "Face3D.h"
#include "Polyhedron.h"
#include "Vector3D.h"

#include <format>

void saveImage(const cv::Mat& image, const std::string& filename) {
    cv::imwrite("../images/" + filename, image);
}

int main() {
    Polyhedron figure = Polyhedron(
{
    {{ {300, 300, 100}, {600, 300, 100}, {600, 600, 100}, {300, 600, 100} }},
    {{ {300, 300, 100}, {300, 600, 100}, {300, 600, 600}, {300, 300, 600} }},
    {{ {300, 300, 100}, {300, 300, 600}, {600, 300, 600}, {600, 300, 100} }},
    {{ {600, 300, 100}, {600, 300, 600}, {600, 600, 600}, {600, 600, 100} }},
    {{ {300, 600, 100}, {600, 600, 100}, {600, 600, 600}, {300, 600, 600} }},
    {{ {300, 300, 600}, {300, 600, 600}, {600, 600, 600}, {600, 300, 600} }}
    });

    auto canvasPerspective = Canvas(1000, 1000);
    for (double angle = 0; angle < 2 * M_PI; angle += M_PI / 50) {
        // очистка
        for (int x = 0; x < canvasPerspective.getWidth(); ++x) {
            for (int y = 0; y < canvasPerspective.getHeight(); ++y) {
                canvasPerspective.drawPoint({static_cast<double>(x), static_cast<double>(y)}, {0, 0, 0});
            }
        }
        auto polys = figure.rotateAroundAxis(angle, {1, 1, 1})
                                         .getPerspectiveOnePointOnZProjectionOnXZ(-0.0006, {0, 0, -1});
        for (const auto& poly : polys) {
            canvasPerspective.drawPolygon(poly, {230, 140, 17});
        }
        saveImage(canvasPerspective.getImage(), std::format("perspective/img{:03d}.png", static_cast<int>(angle * 100)));
    }

    auto canvasParallel = Canvas(1000, 1000);
    for (double angle = 0; angle < 2 * M_PI; angle += M_PI / 50) {
        // очистка
        for (int x = 0; x < canvasParallel.getWidth(); ++x) {
            for (int y = 0; y < canvasParallel.getHeight(); ++y) {
                canvasParallel.drawPoint({static_cast<double>(x), static_cast<double>(y)}, {0, 0, 0});
            }
        }
        auto polys = figure.rotateAroundAxis(angle, {1, 1, 1})
                                         .getParallelProjectionOnXY({0, 0, -1});
        for (const auto& poly : polys) {
            canvasParallel.drawPolygon(poly, {114, 190, 64});
        }
        saveImage(canvasParallel.getImage(), std::format("parallel/img{:03d}.png", static_cast<int>(angle * 100)));
    }

    for (int x = 0; x < canvasParallel.getWidth(); ++x) {
        for (int y = 0; y < canvasParallel.getHeight(); ++y) {
            canvasParallel.drawPoint({static_cast<double>(x), static_cast<double>(y)}, {0, 0, 0});
        }
    }

    for (int x = 0; x < canvasPerspective.getWidth(); ++x) {
        for (int y = 0; y < canvasPerspective.getHeight(); ++y) {
            canvasPerspective.drawPoint({static_cast<double>(x), static_cast<double>(y)}, {0, 0, 0});
        }
    }

    auto parallelPolys = figure.rotateAroundAxis(0.3,{1, 0.5, 1}).getParallelProjectionOnXY();
    for (const auto& poly : parallelPolys) {
        canvasParallel.drawPolygon(poly, {255, 255, 220});
    }

    auto perspectivePolys = figure.rotateAroundAxis(0.3,{1, 0.5, 1}).getPerspectiveOnePointOnZProjectionOnXZ(-0.0006);
    for (const auto& poly : perspectivePolys) {
        canvasPerspective.drawPolygon(poly, {255, 255, 220});
    }

    saveImage(canvasParallel.getImage(), "parallel.png");
    saveImage(canvasPerspective.getImage(), "perspective.png");
    //cv::waitKey(0);
    return 0;
}
