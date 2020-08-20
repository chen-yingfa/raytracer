#ifndef RAY_H
#define RAY_H

#include <cassert>
#include <iostream>
#include <vecmath.h>


// Ray class mostly copied from Peter Shirley and Keith Morley
class Ray {
public:
    Ray() {}
    Ray(const Vector3f &orig, const Vector3f &dir) {
        origin = orig;
        direction = dir;
    }

    Ray(const Ray &r) {
        origin = r.origin;
        direction = r.direction;
    }

    const Vector3f &getOrigin() const { return origin; }

    const Vector3f &getDirection() const {  return direction; }

    Vector3f pointAtParameter(float t) const {
        return origin + direction * t;
    }

private:
    Vector3f origin;
    Vector3f direction;

};

#endif // RAY_H
