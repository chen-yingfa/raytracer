#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include <vecmath.h>
#include <float.h>
#include <cmath>
#include "utils.hpp"

class Camera {
public:
    Camera(const Vector3f& _p, const Vector3f& _z, const Vector3f& _up, int imgW, int imgH, float _aperture = 0.04, float _focusDist = 1.5) {
        pos = _p;
        localZ = _z.normalized();
        localX = Vector3f::cross(localZ, _up).normalized();
        localY = Vector3f::cross(localX, localZ);   // normalized
        width = imgW;
        height = imgH;

        lensRadius = _aperture/2;
        focusDist = _focusDist;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

protected:
    // Extrinsic parameters
    Vector3f pos;
    Vector3f localZ;
    Vector3f localY;
    Vector3f localX;
    // Intrinsic parameters
    int width;
    int height;
    float lensRadius = 0.5;
    float focusDist = 5;
};

// TODO: Implement Perspective camera
// You can add new functions or variables whenever needed.
class PerspectiveCamera : public Camera {
protected:
    float angle;
    Vector3f imgPlaneBottomLeft;
    float imgPlaneWidth;
    float imgPlaneHeight;
public:
    PerspectiveCamera(const Vector3f &center, const Vector3f& _dir,
            const Vector3f &up, int imgW, int imgH, float angle, float _aperture, float _focusDist) : Camera(center, _dir, up, imgW, imgH, _aperture, _focusDist) {
        // angle is in radian.
        this->angle = angle;

        
        imgPlaneWidth = focusDist * tan(angle/2);
        imgPlaneHeight = imgPlaneWidth / width * height;
    }

    Ray generateRay(const Vector2f &point) override {
        Vector2f tmp = lensRadius * Utils::randomInUnitDisk();
        Vector3f offset = localX * tmp.x() + localY * tmp.y();

        Vector3f rayDirX = localX * imgPlaneWidth * (point.x() - width/2) / (width/2);
        Vector3f rayDirY = localY * imgPlaneHeight * (point.y() - height/2) / (height/2);
        Vector3f rayDirZ = localZ * focusDist;

        Vector3f rayDir = rayDirZ + rayDirX + rayDirY - offset;
        return Ray(pos + offset, rayDir.normalized());
    }
};

#endif //CAMERA_H
