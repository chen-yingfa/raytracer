#ifndef MESH_H
#define MESH_H

#include "mesh.hpp"
#include <vector>
#include <vecmath.h>
#include "object3d.hpp"
#include "triangle.hpp"
#include "bvh.hpp"

class BvhNode;

class Mesh : public Object3D {
public:
    Mesh(){}
    Mesh(const char *filename, Material *m);

    struct TriangleIndex {
        TriangleIndex() {
            x[0] = 0; x[1] = 0; x[2] = 0;
        }
        int &operator[](const int i) { return x[i]; }
        // By Computer Graphics convention, counterclockwise winding is front face
        int x[3]{};
    };

    vector<Triangle*>& getTriangles() { return triangles; }
    int getMeshSize() { return triangles.size(); }

    bool intersect(const Ray &r, Hit &h, float tmin, float tmax) override;

    bool hitbox(Aabb& box) const;
    BvhNode *tree;
private:
    // Normal can be used for light estimation
    void computeTriangles(vector<Vector3f>& vertices, vector<Vector3f>& faces);
    std::vector<Triangle*> triangles;
};

#endif
