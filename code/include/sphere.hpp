#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

class Sphere : public Object3D {
public:
    Sphere() {
        // unit ball at the center
        radius = 1.0;
        center = Vector3f::ZERO;
    }

    Sphere(const Vector3f &center, float radius, Material *material) : Object3D(material) {
        this->center = center;
        this->radius = radius;
        objType = sphere;
    }

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin, float tmax) override {
        // cout << "intersect on sphere\n";
        Vector3f rayDir = r.getDirection().normalized();
        Vector3f oc = center - r.getOrigin();
        float ocLen = oc.length();
        float oh = Vector3f::dot(oc, rayDir);
        float ch2 = ocLen * ocLen - oh * oh;
        float discriminant = radius * radius - ch2;
        if (discriminant < 0) { // do not intersect
            return false;
        }

        float t;
        if (oc.length() > radius) {     // ray from outside sphere
            t = oh - sqrt(discriminant);
        } else {               // ray from inside sphere
            t = oh + sqrt(discriminant);
        }

        if (t < tmin || h.getT() < t) {
            return false;
        }
        
        Vector3f p = r.pointAtParameter(t);
        h.set(p, t, material);
        Vector3f normal = (p - center).normalized();
        h.setNormal(r, normal);
        float u, v;
        getUvSphere(normal, u, v);
        h.setUv(u, v);
        // cout << "done intersecting\n";
        return true;
    }

    bool hitbox(Aabb& box) const {
        float r = abs(radius);
        box = Aabb(center - Vector3f(r, r, r), 
                    center + Vector3f(r, r, r));
        return true;
    }

    // 输出信息
    void print() {
        cout << "Sphere, radius: " << radius << " ";
        center.print();
    }

    // 计算球面上的uv值
    void getUvSphere(const Vector3f& pos, float& u, float& v) {
        float phi = atan2(pos.z(), pos.x());
        float theta = asin(pos.y());
        u = 0.5 - phi / (2 * PI);
        v = theta/PI + 0.5;
    }

protected:
    Vector3f center;
    float radius;
};


#endif
