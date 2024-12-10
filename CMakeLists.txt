cmake_minimum_required(VERSION 3.30)
project(valentovCG)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        "D:\\Libraries\\OpenCV-MinGW-Build\\include"
        ".\\lib\\include"
)

set(OpenCV_DIR "D:\\Libraries\\OpenCV-MinGW-Build")
set(CMAKE_PREFIX_PATH "${OPENCV_DIR}" ${CMAKE_PREFIX_PATH})

find_package(OpenCV REQUIRED)

add_executable(cg main.cpp
        lib/include/Vector2D.h
        lib/source/Vector2D.cpp
        lib/source/Polygon.cpp
        lib/include/Canvas.h
        lib/source/Canvas.cpp
        lib/include/Face3D.h
        lib/include/Vector3D.h
        lib/source/Vector3D.cpp
        lib/source/Face3D.cpp
        lib/include/Polyhedron.h
        lib/source/Polyhedron.cpp
)

target_link_libraries(cg ${OpenCV_LIBS})
