#ifndef BVH_H
#define BVH_H

#include <algorithm>
#include "object3d.hpp"
#include "group.hpp"
#include "mesh.hpp"
#include "aabb.hpp"

inline bool boxCmp(const Object3D* a, const Object3D* b, int dim) {
    Aabb boxA;
    Aabb boxB;

    if (!a->hitbox(boxA) || !b->hitbox(boxB))
        std::cerr << "Error: called boxCmp on objects without bounding box\n";

    return boxA.getMin()[dim] < boxB.getMin()[dim];
}

inline bool boxCmpX(const Object3D* a, const Object3D* b) {
    return boxCmp(a, b, 0);
}

inline bool boxCmpY(const Object3D* a, const Object3D* b) {
    return boxCmp(a, b, 1);
}

inline bool boxCmpZ(const Object3D* a, const Object3D* b) {
    return boxCmp(a, b, 2);
}


class BvhNode : public Object3D {
public:
    BvhNode(){}
    BvhNode(Group *grp) : BvhNode(grp->getObjects(), 0, grp->getGroupSize()) {}
    // BvhNode(Mesh *m) : BvhNode(m->getTriangles(), 0, m->getMeshSize()) {}
    BvhNode(std::vector<Object3D*>& objects, int lo, int hi);
    BvhNode(std::vector<Triangle*>& triangles, int lo, int hi);

    

    virtual bool intersect(const Ray& ray, Hit& hit, float tmin, float tmax) override;
    virtual bool hitbox(Aabb& box) const;

public:
    Object3D* left;
    Object3D* right;
    Aabb box;
};

#endif // BVH_H