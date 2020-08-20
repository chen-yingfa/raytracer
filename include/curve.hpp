#ifndef CURVE_HPP
#define CURVE_HPP

#include "object3d.hpp"
#include <vecmath.h>
#include <vector>
#include <utility>

#include <algorithm>
// 从PA3复制过来
class Curve : public Object3D {
protected:
    std::vector<Vector3f> controls;
public:
    explicit Curve(std::vector<Vector3f> points) : controls(std::move(points)) {}

    bool intersect(const Ray &r, Hit &h, float tmin, float tmax) override {
        return false;  // 永远不相交
    }

    std::vector<Vector3f> &getControls() { return controls; }

    bool hitbox(Aabb& box) const { return true; } // 好像是错？

    virtual float B(float t, int i, int n) = 0;
    virtual float BDeriv(float t, int i, int n) = 0;
};

// Bezier曲线
class BezierCurve : public Curve {
public:
    explicit BezierCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4 || points.size() % 3 != 1) {
            printf("Number of control points of BezierCurve must be 3n+1!\n");
            exit(0);
        }
    }

    float BDeriv(float t, int i, int n) {
        float ret = B(t, i-1, n-1) - B(t, i, n-1);
        return n * ret;
    }

    float B(float t, int i, int n) {
        if (i < 0 || n < i) return 0.0f;
        float ret = (float) nCr(n, i);
        // ret *= t ^ i
        for (int j = 0; j < i; j++){
            ret *= t;
        }
        // ret *= t ^ (n - i)
        for (int j = 0; j < n-i; j++){
            ret *= (1-t);
        }

        return ret;
    }

    long long int nCr(int n, int r) {
        long long* row = new long long[n + 1]; // DP
        for (int i = 0; i <= n; i++) row[i] = 0;

        row[0] = 1;
        for (int i = 1; i <= n; i++){
            for (int j = i; j > 0; j--) {
                row[j] += row[j-1];
            }
        }
        float ret = row[r];
        delete [] row; // free memory
        return ret;
    }

protected:
};

#endif // CURVE_HPP
