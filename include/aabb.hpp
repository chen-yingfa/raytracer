#ifndef AABB_H
#define AABB_H

#include <vecmath.h>
#include <iostream>
#include "ray.hpp"

class Aabb {
public:
    Aabb(){}
    Aabb(const Vector3f& _mn, const Vector3f& _mx) : mn(_mn), mx(_mx) {}

    Vector3f getMin() const { return mn; }
    Vector3f getMax() const { return mx; }

    inline bool intersect(const Ray& r, float tmin, float tmax) const {
        for (int i = 0; i < 3; i++){
            float inv = 1.0f / r.getDirection()[i];
            float t0 = (mn[i] - r.getOrigin()[i]) * inv;
            float t1 = (mx[i] - r.getOrigin()[i]) * inv;
            if (inv < 0.0f)
                std::swap(t0, t1);
            tmin = t0 > tmin ? t0 : tmin;
            tmax = t1 < tmax ? t1 : tmax;
            if (tmax <= tmin) return false; // for infinity case, tmax == tmin
        }
        return true;
    }

    static Aabb surroundingBox(Aabb a, Aabb b) {
        // returns the AABB surrounding both a and b
        Vector3f min0 = a.getMin();
        Vector3f max0 = a.getMax();
        Vector3f min1 = b.getMin();
        Vector3f max1 = b.getMax();
        Vector3f mn(fmin(min0.x(), min1.x()),
                    fmin(min0.y(), min1.y()),
                    fmin(min0.z(), min1.z()));
        Vector3f mx(fmax(max0.x(), max1.x()),
                    fmax(max0.y(), max1.y()),
                    fmax(max0.z(), max1.z()));
        return Aabb(mn, mx);
    }

    void print() {
        std::cout << "AABB:\n  ";
        mn.print();
        std::cout << "  ";
        mx.print();
    }

protected:
    Vector3f mn, mx;
};

#endif // AABB_H