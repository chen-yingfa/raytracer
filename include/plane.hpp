#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions
class Plane : public Object3D {
public:
    Plane() {}

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        this->normal = normal;
        this->d = d;
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin, float tmax) override {
        float t = (d - Vector3f::dot(normal, r.getOrigin())) / Vector3f::dot(normal, r.getDirection());

        if (tmin < t && t < h.getT()) {
            Vector3f p = r.pointAtParameter(t);
            h.set(p, t, material);
            h.setNormal(r, normal);
            return true;
        }
        return false;
    }

    bool hitbox(Aabb& box) const { return false; } // 无包围盒

protected:
    Vector3f normal;
    float d;
};

#endif //PLANE_H
		

