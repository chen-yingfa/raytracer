#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "object3d.hpp"
#include "triangle.hpp"

// 三个轴向的矩形

class RectX : public Object3D {
public:
    RectX(){}
    RectX(const Vector2f& mn, const Vector2f& mx, float _d, Material* m) 
        : y0(mn.x()), y1(mx.x()), z0(mn.y()), z1(mx.y()), d(_d), Object3D(m) {
        objType = rectX;
    }
    RectX(float _x0, float _x1, float _y0, float _y1, float _d, Material* m) 
        : y0(_x0), y1(_x1), z0(_y0), z1(_y1), d(_d), Object3D(m) {
        objType = rectX;
    }

    // 求交并将信息存到 hit 中
    virtual bool intersect(const Ray& ray, Hit& hit, float tmin, float tmax) {
        if (ray.getDirection().x() == 0) return false; // parallell to the plane => assume no intersection
        float t = (d - ray.getOrigin().x()) / ray.getDirection().x();  
        if (t < tmin || hit.getT() < t) return false;    // limited by tmin and current closest hit
        
        Vector3f p = ray.pointAtParameter(t);
        float y = p.y();
        float z = p.z();
        if (y < y0 || y1 < y || z < z0 || z1 < z) return false;   // doesn't cross the plane

        // 保存信息到hit
        hit.setU((z-z0) / (z1-z0));
        hit.setV((y-y0) / (y1-y0));
        hit.set(p, t, material);
        hit.setNormal(ray, Vector3f(1, 0, 0));
        return true;
    }

    virtual bool hitbox(Aabb& box) const {
        box = Aabb(Vector3f(d-0.0001, y0, z0), Vector3f(d+0.0001, y1, z1)); // thin box, must have some volume
        return true; // has bounding box 
    }

    void setMat(Material* m) { material = m;}

private:
    float y0, y1, z0, z1;
    float d;
};

// 以下两类RectY，RectZ跟上面的RectX一模一样， 只是换了个方向。

class RectY : public Object3D {
public:
    RectY(){}
    RectY(const Vector2f& mn, const Vector2f& mx, float _d, Material* m) 
        : x0(mn.x()), x1(mx.x()), z0(mn.y()), z1(mx.y()), d(_d), Object3D(m) {
        objType = rectY;
    }
    RectY(float _x0, float _x1, float _y0, float _y1, float _d, Material* m) 
        : x0(_x0), x1(_x1), z0(_y0), z1(_y1), d(_d), Object3D(m) {
        objType = rectY;
    }

    virtual bool intersect(const Ray& ray, Hit& hit, float tmin, float tmax) {
        if (ray.getDirection().y() == 0) return false; // parallell to the plane => assume no intersection
        float t = (d - ray.getOrigin().y()) / ray.getDirection().y();  
        if (t < tmin || hit.getT() < t) return false;    // limited by tmin and current closest hit
        
        Vector3f p = ray.pointAtParameter(t);
        float x = p.x();
        float z = p.z();
        if (x < x0 || x1 < x || z < z0 || z1 < z) return false;   // doesn't cross the plane

        // save info in Hit object
        hit.setU((x-x0) / (x1-x0));
        hit.setV((z-z0) / (z1-z0));
        hit.set(p, t, material);
        hit.setNormal(ray, Vector3f(0, 1, 0));
        return true;
    }

    virtual bool hitbox(Aabb& box) const {
        box = Aabb(Vector3f(x0, d-0.0001, z0), Vector3f(x1, d+0.0001, z1)); // thin box, must have some volume
        return true; // has bounding box 
    }

private:
    float x0, x1, z0, z1;
    float d;
};


class RectZ : public Object3D {
public:
    RectZ(){}
    RectZ(const Vector2f& mn, const Vector2f& mx, float _d, Material* m) 
        : x0(mn.x()), x1(mx.x()), y0(mn.y()), y1(mx.y()), d(_d), Object3D(m) {
        objType = rectZ;
    }

    RectZ(float _x0, float _x1, float _y0, float _y1, float _d, Material* m) 
        : x0(_x0), x1(_x1), y0(_y0), y1(_y1), d(_d), Object3D(m) {
        objType = rectZ;
    }

    virtual bool intersect(const Ray& ray, Hit& hit, float tmin, float tmax) {
        if (ray.getDirection().z() == 0) return false; // parallell to the plane => assume no intersection
        float t = (d - ray.getOrigin().z()) / ray.getDirection().z();  
        if (t < tmin || hit.getT() < t) return false;    // limited by tmin and current closest hit
        
        Vector3f p = ray.pointAtParameter(t);
        float x = p.x();
        float y = p.y();
        if (x < x0 || x1 < x || y < y0 || y1 < y) return false;   // doesn't cross the plane

        // save info in Hit object
        hit.setU((x-x0) / (x1-x0));
        hit.setV((y-y0) / (y1-y0)); // render upside down along y-axis
        hit.set(p, t, material);
        hit.setNormal(ray, Vector3f(0, 0, 1));
        return true;
    }

    virtual bool hitbox(Aabb& box) const {
        box = Aabb(Vector3f(x0, y0, d-0.0001), Vector3f(x1, y1, d+0.0001)); // thin box, must have some volume
        return true; // has bounding box 
    }

private:
    float x0, x1, y0, y1;
    float d;
};


#endif // RECTANGLE_H