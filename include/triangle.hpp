#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include "plane.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>
using namespace std;

class Triangle: public Object3D {

public:
	Triangle() = delete;

    // a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) 
	: Object3D(m) {
		this->a = a;
		this->b = b;
		this->c = c;
		objType = triangle;
	}

	bool intersect( const Ray& ray,  Hit& hit , float tmin, float tmax) override {
		float dotProd = Vector3f::dot(normal, ray.getDirection());
		if (dotProd == 0) return false;   // ray is perpendicular to normal => assume no intersection
		float d = Vector3f::dot(normal, a);
		float t = (d - Vector3f::dot(normal, ray.getOrigin())) / dotProd;
		if (t < tmin || hit.getT() < t) {
			return false;
		}

		// check if is inside triangle
		Vector3f p = ray.pointAtParameter(t); // intersection pt of ray and plane of triangle
		Vector3f pn = Vector3f::cross(b - p, c - p);
		if (Vector3f::dot(pn, normal) < 0) { // p is not in the triangle
			return false;
		}
		pn = Vector3f::cross(c - p, a - p);
		if (Vector3f::dot(pn, normal) < 0) { // p is not in the triangle
			return false;
		}
		pn = Vector3f::cross(a - p, b - p);
		if (Vector3f::dot(pn, normal) < 0) { // p is not in the triangle
			return false;
		}
		// p is in the triangle

		float longestSide = 2 * fmax((c-a).length(), (b-a).length()) + 0.001;
		if (longestSide == 0) return false;   // triangle is infinitely small
		float u = (p - a).length() / longestSide;
		float v = (p - b).length() / longestSide;
		hit.set(p, t, material);
		hit.setNormal(ray, normal);
		hit.setUv(u, v);
        return true;
	}

	// set box as the hitbox of this triangle
	// returns whether hitbox exists (always true for Triangles)
	bool hitbox(Aabb& box) const {
        float minX = fmin(a.x(), fmin(b.x(), c.x()));
		float minY = fmin(a.y(), fmin(b.y(), c.y()));
		float minZ = fmin(a.z(), fmin(b.z(), c.z()));
		float maxX = fmax(a.x(), fmax(b.x(), c.x()));
		float maxY = fmax(a.y(), fmax(b.y(), c.y()));
		float maxZ = fmax(a.z(), fmax(b.z(), c.z()));
		Vector3f small(0.001, 0.001, 0.001);
		box = Aabb(Vector3f(minX, minY, minZ) - small, Vector3f(maxX, maxY, maxZ) + small);
		return true;
    }

	void setNormal(const Vector3f n) { normal = n; }

protected:
	Vector3f normal;
	Vector3f a, b, c;
};

#endif //TRIANGLE_H
