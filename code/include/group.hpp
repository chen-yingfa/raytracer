#ifndef GROUP_H
#define GROUP_H

#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>

// class Mesh;

// TODO: Implement Group - add data structure to store a list of Object*
class Group : public Object3D {
public:
    Group() {

    }

    explicit Group (int num_objects) {
        objects = std::vector<Object3D*>(num_objects);
        objType = group;
    }

    ~Group() override {
        // 需要手动释放空间吗？
        for (auto obj : objects){
            delete obj;
        }
    }

    bool intersect(const Ray &r, Hit &h, float tmin, float tmax) override {
        bool result = false;
        for (int i = 0; i < objects.size(); ++i){
            result |= objects[i]->intersect(r, h, tmin, h.getT());
        }
        return result;
    }

    bool hitbox(Aabb& box) const {
        if (objects.empty()) return false;

        Aabb curBox;
        bool firstBox = true;

        for (const Object3D* obj : objects) {
            if (!obj->hitbox(curBox)) return false;
            box = firstBox ? curBox : Aabb::surroundingBox(box, curBox);
            firstBox = false;
        }

        return true;
    }

    void addObject(int index, Object3D *obj) {
        objects[index] = obj;
    }

    void addObject(Object3D* obj) {
        objects.push_back(obj);
    }

    int getGroupSize() {
        return objects.size();
    }

    std::vector<Object3D*>& getObjects(){
        return objects;
    }
private:
    std::vector<Object3D*> objects;
};

#endif
	
