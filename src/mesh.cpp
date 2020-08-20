#include "mesh.hpp"  // declarator
#include <fstream>   // file stream
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <utility>
#include <sstream>   // read obj file

bool Mesh::intersect(const Ray &r, Hit &h, float tmin, float tmax) {
    return tree->intersect(r, h, tmin, tmax);
}

Mesh::Mesh(const char *filename, Material *material) : Object3D(material) {
    std::vector<Vector3f> vertices;
    std::vector<Vector3f> faces;

    // Optional: Use tiny obj loader to replace this simple one.
    std::ifstream f;
    f.open(filename);
    if (!f.is_open()) {
        std::cout << "Cannot open " << filename << "\n";
        return;
    }
    std::string line;
    std::string vTok("v");
    std::string fTok("f");
    std::string texTok("vt");
    char bslash = '/', space = ' ';
    std::string tok;
    int texID;
    while (true) {
        std::getline(f, line);
        if (f.eof()) {
            break;
        }
        if (line.size() < 3) {
            continue;
        }
        if (line.at(0) == '#') {
            continue;
        }
        std::stringstream ss(line);
        ss >> tok;
        if (tok == vTok) {
            Vector3f vec;
            ss >> vec[0] >> vec[1] >> vec[2];
            vertices.push_back(vec);
        } else if (tok == fTok) {
            if (line.find(bslash) != std::string::npos) {
                std::replace(line.begin(), line.end(), bslash, space);
                std::stringstream facess(line);
                Vector3f trig;
                facess >> tok;
                for (int ii = 0; ii < 3; ii++) {
                    facess >> trig[ii] >> texID;
                    trig[ii]--;
                }
                faces.push_back(trig);
            } else {
                Vector3f trig;
                for (int ii = 0; ii < 3; ii++) {
                    ss >> trig[ii];
                    trig[ii]--;
                }
                faces.push_back(trig);
            }
        } else if (tok == texTok) {
            Vector2f texcoord;
            ss >> texcoord[0];
            ss >> texcoord[1];
        }
    }
    f.close();

    computeTriangles(vertices, faces);

    objType = mesh;
    tree = new BvhNode(triangles, 0, triangles.size());

    cout << "Loaded Mesh with " << triangles.size() << " trianges\n";
}

void Mesh::computeTriangles(vector<Vector3f>& vertices, vector<Vector3f>& faces) {
    triangles = vector<Triangle*>(faces.size());
    Vector3f normal;
    for (int i = 0; i < faces.size(); i++) {
        Vector3f& face = faces[i];
        Vector3f v0 = vertices[face[0]];
        Vector3f v1 = vertices[face[1]];
        Vector3f v2 = vertices[face[2]];

        triangles[i] = new Triangle(v0, v1, v2, material);
        Vector3f a = v1 - v0;
        Vector3f b = v2 - v0;
        b = Vector3f::cross(a, b);
        triangles[i]->setNormal(b.normalized());
    }
}

bool Mesh::hitbox(Aabb& box) const {
    return tree->hitbox(box);
}
