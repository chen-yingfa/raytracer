CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(PA1 CXX)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

ADD_SUBDIRECTORY(deps/vecmath)

SET(PA1_SOURCES
        src/bvh.cpp
        src/image.cpp
        src/main.cpp
        src/mesh.cpp
        src/scene_parser.cpp
        src/texture.cpp
        )

SET(PA1_INCLUDES
        include/aabb.hpp
        include/bvh.hpp
        include/camera.hpp
        include/curve.hpp
        include/group.hpp
        include/hit.hpp
        include/image.hpp
        include/light.hpp
        include/material.hpp
        include/mesh.hpp
        include/object3d.hpp
        include/plane.hpp
        include/ray.hpp
        include/rectangle.hpp
        include/revsurface.hpp
        include/scene_parser.hpp
        include/sceneGenerator.hpp
        include/sphere.hpp
        include/texture.hpp
        include/transform.hpp
        include/triangle.hpp
        include/utils.hpp
        )

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

ADD_EXECUTABLE(${PROJECT_NAME} ${PA1_SOURCES} ${PA1_INCLUDES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} vecmath)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE include)
