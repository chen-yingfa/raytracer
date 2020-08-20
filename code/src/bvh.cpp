
#include "bvh.hpp"
#include <vector>
#include "object3d.hpp"
#include "mesh.hpp"
#include "group.hpp"

bool BvhNode::intersect(const Ray& ray, Hit& hit, float tmin, float tmax) {
    if (!box.intersect(ray, tmin, tmax)) {
        return false; // 若不与本节点的box交
    }

    bool intersectLeft = left->intersect(ray, hit, tmin, tmax);
    bool intersectRight;
    if (intersectLeft) {
        intersectRight = right->intersect(ray, hit, tmin, hit.getT());
    } else {
        intersectRight = right->intersect(ray, hit, tmin, tmax);
    }

    return intersectLeft || intersectRight;
}

// Construct BVH Tree for a Group
BvhNode::BvhNode(std::vector<Object3D*>& objects, int lo, int hi) {
    // comparator for sorting (random pick one axis)
    int axis = Utils::randomInt(0, 2); 
    auto cmp = (axis == 0) ? boxCmpX
             : (axis == 1) ? boxCmpY
             : boxCmpZ;
    
    int numObj = hi - lo;

    if (numObj == 1) {          // end case, create leaf 
        left = right = objects[lo]; // duplicated of one object
    } else if (numObj == 2) {   // end case, create leaf
        if (boxCmp(objects[lo], objects[lo+1], axis)) {
            left = objects[lo];
            right = objects[lo+1];
        } else {
            left = objects[lo+1];
            right = objects[lo];
        }
    } else {  // general case
        std::sort(objects.begin() + lo, objects.begin() + hi, cmp);
        int mid = (lo + hi) / 2;
        left =  new BvhNode(objects, lo, mid);
        right = new BvhNode(objects, mid, hi);
    }

    // compute hitbox
    Aabb boxLeft, boxRight;
    if (!left->hitbox(boxLeft) || !right->hitbox(boxRight)){
        std::cerr << "Error: Attempted contructing BVH node on objects without bounding box\n";
        exit(0); // force all objects to have hitbox
    }
    
    box = Aabb::surroundingBox(boxLeft, boxRight);
}

// Construct BVH Tree for a Mesh，跟给Group建立树步骤一样
BvhNode::BvhNode(std::vector<Triangle*>& triangles, int lo, int hi) {
    // comparator for sorting (random pick one axis)
    int axis = Utils::randomInt(0, 2); 
    auto cmp = (axis == 0) ? boxCmpX
             : (axis == 1) ? boxCmpY
             : boxCmpZ;
    
    int numObj = hi - lo;

    if (numObj == 1) {          // end case, create leaf 
        left = right = triangles[lo]; // duplicated of one object
    } else if (numObj == 2) {   // end case, create leaf
        if (boxCmp(triangles[lo], triangles[lo+1], axis)) {
            left = triangles[lo];
            right = triangles[lo+1];
        } else {
            left = triangles[lo+1];
            right = triangles[lo];
        }
    } else {  // general case
        std::sort(triangles.begin() + lo, triangles.begin() + hi, cmp);
        int mid = (lo + hi) / 2;
        left = new BvhNode(triangles, lo, mid);
        right = new BvhNode(triangles, mid, hi);
    }

    // compute hitbox
    Aabb boxLeft, boxRight;
    if (!left->hitbox(boxLeft) || !right->hitbox(boxRight)){
        std::cerr << "Error: Attempted contructing BVH node on objects without bounding box\n";
        exit(0); // force all objects to have hitbox
    }

    box = Aabb::surroundingBox(boxLeft, boxRight);
    // printf("box\n");
    // box.print();
}

// will have computed hitbox, because BVH tree is constructed in the constructor function
bool BvhNode::hitbox(Aabb& box) const {
    box = this->box;
    return true;
}
