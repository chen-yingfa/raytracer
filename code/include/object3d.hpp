#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "ray.hpp"
#include "hit.hpp"
#include "material.hpp"
#include "aabb.hpp"

enum ObjectType {group, mesh, sphere, rectX, rectY, rectZ, triangle, bhvNode, aabb};

// Base class for all 3d entities.
class Object3D {
public:
    Object3D() : material(nullptr) {}

    virtual ~Object3D() = default;

    explicit Object3D(Material *material) {
        this->material = material;
    }

    // Intersect Ray with this object. If hit, store information in hit structure.
    virtual bool intersect(const Ray &r, Hit &h, float tmin, float tmax) = 0;

    // Computes the bounding box
    virtual bool hitbox(Aabb& box) const = 0;

    void setMat( Material* m ) { material = m;}

    ObjectType objType;
protected:
    Material *material;
};

#endif

