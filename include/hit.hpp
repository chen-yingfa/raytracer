#ifndef HIT_H
#define HIT_H

#include <vecmath.h>
#include "ray.hpp"

class Material;

class Hit {
public:
    // constructors
    Hit() {
        material = nullptr;
        t = 1e38;
    }

    Hit(Vector3f _pos, float _t, Material *m, const Vector3f &n) {
        pos = _pos;
        t = _t;
        material = m;
        normal = n;
    }

    Hit(const Hit &h) {
        pos = h.pos;
        t = h.t;
        material = h.material;
    }

    // destructor
    ~Hit() = default;

    float getT() const { return t; }
    float getU() const { return u; }
    float getV() const { return v; }

    const Vector3f getPos() const {return pos;}

    Material *getMaterial() const {
        return material;
    }

    const Vector3f &getNormal() const {
        return normal;
    }

    const bool getIsOuter() const {
        return isOuter;
    }

    inline void setNormal(const Ray& ray, const Vector3f& outwardNormal) {
        isOuter = Vector3f::dot(ray.getDirection(), outwardNormal) < 0;
        normal = isOuter ? outwardNormal : -outwardNormal;
    }

    void set(Vector3f _pos, float _t, Material *m) {
        pos = _pos;
        t = _t;
        material = m;
    }
    void setU(float x) {u=x;}
    void setV(float x) {v=x;}
    void setUv(const float _u, const float _v) {
        u = _u;
        v = _v;
    }
private:
    Vector3f pos;
    float t; // ray's t value at pos
    float u; // UV value for texturing 
    float v; // UV value for texturing
    Material *material;
    Vector3f normal; // always pointing towards the side the ray is coming from
    bool isOuter;    // whether normal is pointing outwards (in terms of the object)
};

inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
    return os;
}

#endif // HIT_H
