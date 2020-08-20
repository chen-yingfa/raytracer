#ifndef BOX_H
#define BOX_H

#include <vecmath.h>
#include <vector>
#include "object3d.hpp"
#include "rectangle.hpp"

class Box : public Object3D {
public:
    Box (){}
    Box(const Vector3f& v0, const Vector3f& v1, Material* m) : Object3D(m) {
        mn = v0;
        mx = v1;
        back = new RectZ(v0.x(), v1.x(), v0.y(), v1.y(), v0.z(), m);
        front = new RectZ(v0.x(), v1.x(), v0.y(), v1.y(), v1.z(), m);
        bottom = new RectY(v0.x(), v1.x(), v0.z(), v1.z(), v0.y(), m);
        top = new RectY(v0.x(), v1.x(), v0.z(), v1.z(), v1.y(), m);
        left = new RectX(v0.y(), v1.y(), v0.z(), v1.z(), v0.x(), m);
        right = new RectX(v0.y(), v1.y(), v0.z(), v1.z(), v1.x(), m);
    
        // 上下左右前后
        faces.resize(6);
        faces[0] = top;
        faces[1] = bottom;
        faces[2] = left;
        faces[3] = right;
        faces[4] = front;
        faces[5] = back;
    }

    bool intersect(const Ray& ray, Hit& hit, float tmin, float tmax) {
        bool ret = false;
        for (Object3D* face : faces){
            ret |= face->intersect(ray, hit, tmin, tmax);
        }
        return ret;
    }

    bool hitbox(Aabb& box) const {
        box = Aabb(mn, mx);
        return true;
    }

    void setTop(Material* m) { top->setMat(m);}
    void setBottom(Material* m) { bottom->setMat(m); }
    void setSides(Material* m) {
        for (int i = 2; i < 6; i++) {
            faces[i]->setMat(m);
        }
    }

private:
    Vector3f mn, mx;
    RectX *left, *right;
    RectY *top, *bottom;
    RectZ *front, *back;
    vector<Object3D*> faces;
};

#endif // BOX_H