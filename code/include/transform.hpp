#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "object3d.hpp"

// transforms a 3D point using a matrix, returning a 3D point
static Vector3f transformPoint(const Matrix4f &mat, const Vector3f &point) {
    return (mat * Vector4f(point, 1)).xyz();
}

// transform a 3D direction using a matrix, returning a direction
static Vector3f transformDirection(const Matrix4f &mat, const Vector3f &dir) {
    return (mat * Vector4f(dir, 0)).xyz();
}

// TODO: implement this class so that the intersect function first transforms the ray
class Transform : public Object3D {
public:
    Transform() {}

    Transform(const Matrix4f &m, Object3D *obj) : o(obj) {
        transform = m;
        inverse = m.inverse();
        objType = obj->objType;
    }

    ~Transform() {
    }

    virtual bool intersect(const Ray &r, Hit &h, float tmin, float tmax) {
        // printf("intersect on transform\n");
        Vector3f trSource = transformPoint(inverse, r.getOrigin());
        Vector3f trDirection = transformDirection(inverse, r.getDirection());
        Ray tr(trSource, trDirection);
        bool intersected = o->intersect(tr, h, tmin, tmax);
        if (!intersected) return false;
        
        Vector3f pos = h.getPos();
        Vector3f n = h.getNormal();
        pos = transformPoint(transform, pos);
        n = transformDirection(transform, n).normalized();

        h.set(pos, h.getT(), h.getMaterial());
        h.setNormal(tr, n);

        return true;
    }

    bool hitbox(Aabb& box) const {
        // calculate and set box as the AABB (hitbox) of this Transform object
        if (!o->hitbox(box)) return false;
        Vector3f mn = box.getMin();
        Vector3f mx = box.getMax();
        mn = transformPoint(transform, mn);
        mx = transformPoint(transform, mx);
        box = Aabb(mn, mx);
        return true; // always return true when this object has an AABB
    }

protected:
    Object3D *o; //un-transformed object
    Matrix4f inverse;
    Matrix4f transform;
};

#endif //TRANSFORM_H
