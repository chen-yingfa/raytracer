#ifndef REVSURFACE_HPP
#define REVSURFACE_HPP

#include "object3d.hpp"
#include "curve.hpp"
#include "ray.hpp"
#include <vecmath.h>
#include <tuple>
#include <vector>

// Bezier Surface of revolution about y axis, control points are on xy plane
class RevSurface : public Object3D {
public:
    Curve *pCurve;
    Aabb aabb;
public:
    RevSurface(Curve *pCurve, Material* material) : pCurve(pCurve), Object3D(material) {
        // Make sure all points are on xy plane
        for (const auto &cp : pCurve->getControls()) {
            if (cp.z() != 0.0) {
                printf("All control points of revSurface must be on xy plane.\n");
                exit(0);
            }
        }

        // 计算包围盒 (AABB)，TODO：分段加速？
        float maxX, minY, maxY;
        vector<Vector3f> cp = pCurve->getControls();
        minY = maxY = cp[0].y();
        // only save max x coordinate, because this surface revolves 
        // around y axis, and control points are all on xy plane
        maxX = fabs(cp[0].x());
        if (cp.size() == 1) {    // 只有一个控制节点
            Vector3f small(0.0001, 0.0001, 0.0001);
            Vector3f mn(-maxX, minY, -maxX);
            Vector3f mx(maxX, minY, maxX);
            aabb = Aabb(mn - small, mx + small);
        } else {                // 一般情况
            for (Vector3f& p : cp) {
                minY = fmin(minY, p.y());
                maxY = fmax(maxY, p.y());

                maxX = fmax(maxX, fabs(p.x())); // always on positive side
            }
            Vector3f mn(-maxX, minY, -maxX);
            Vector3f mx(maxX, maxY, maxX);
            aabb = Aabb(mn, mx);
        }
    }

    ~RevSurface() override {
        delete pCurve;
    }

    bool intersect(const Ray &ray, Hit &hit, float tmin, float tmax) override {
        if (!aabb.intersect(ray, tmin, tmax)) return false; 

        // 一元牛顿法
        float u;
        if (!newtonOne(ray, u)) return false;  // 没找到根

        Vector3f p = P(u);
        float t = (p.y() - ray.getOrigin().y()) / ray.getDirection().y();
        if (t < tmin || hit.getT() < t) return false;  // 要求在 tmin 和 hit 的最小的 t 之间
        Vector3f pos = ray.pointAtParameter(t);
        float v = acos(pos.x() / p.x());
        if (v < 0) v += 2 * PI; 

        Vector3f tangentU = pointAtu(u, v);
        Vector3f tangentV = pointAtv(u, v);
        Vector3f outwardNormal = Vector3f::cross(tangentV, tangentU);

        // 保存hit信息
        hit.set(pos, t, material);
        hit.setNormal(ray, outwardNormal);
        hit.setUv(u, v);
        return true;
    }

    bool hitbox(Aabb& box) const {
        // RevSurface always computes hitbox in constructor
        box = aabb;
        return true;
    }

    // 牛顿法求ray和参数曲面交点
    bool newtonOne(const Ray& ray, float& root) {
        Vector3f d = ray.getDirection().normalized();
        Vector3f o = ray.getOrigin();
        // helpers
        float o_r, d_r;
        o_r = distFromYAxis(o);
        d_r = distFromYAxis(d);

        float maxError = 0.0001;
        int patience = 4;
        int worseCnt = 0;
        float yDist = 0;
        bool converged = false;
        for (float start = 0.0001; start < 1; start += 0.099) {
            float u0 = start;
            // float u0 = Utils::randomFloat(0.0001, 0.9999); // pick a random starting point
            float fu = f(u0, ray);
            float fuprime = fprime(u0, ray);
            float u = u0 - fu / fuprime;
            u = Utils::clamp(u, 0.0001, 0.9999);   // u has to be in [0, 1]
            float prev = 0;  // used to check whether is converging 
            for (int j = 0; j < 20; j++) {
                fu = f(u, ray);
                if (fabs(fu) < maxError) {
                    float tmpY = fabs(P(u).y() - o.y());
                    if (!converged) { // found first root
                        root = u;
                        yDist = tmpY;
                        converged = true;
                    } else {
                        if (tmpY < yDist) { 
                            // will only process the newly found root 
                            // if it's closer to the ray's origin
                            yDist = tmpY;
                            root = u;
                        }
                    }
                } else if (worseCnt > patience) {
                    worseCnt = 0;
                    break;
                }
                // iterate 
                u0 = u;
                fuprime = fprime(u, ray);

                u = u0 - fu / fuprime;
                u = Utils::clamp(u, 0.001, 0.999);    // u has to be in [0, 1]
                if (fu > prev - 0.0001) ++worseCnt;   // got worse result
                else worseCnt = 0;                    // got better result
                prev = fu;
            }
        }
        if (!converged) return false;   // 没有找到根
        return true;
    }

    // 返回在u对应的与y垂直的平面上，射线与参数曲面的距离
    float f(float u, const Ray& r) {
        Vector3f p = P(u);
        Vector3f o = r.getOrigin(), d = r.getDirection();
        float ts = (p.y() - o.y()) / d.y();
        Vector3f rs = r.pointAtParameter(ts);
        float dr = distFromYAxis(rs);
        dr -= fabs(p.x());
        return dr;
    }

    // 上述f的倒数
    float fprime(float u, const Ray& r) {
        Vector3f p = P(u);
        Vector3f pprime = PDeriv(u);
        Vector3f o = r.getOrigin(), d = r.getDirection();
        float ts = (p.y() - o.y()) / d.y();
        Vector3f rs = r.pointAtParameter(ts);
        // flattent p and rs on xz plane
        p.y() = 0;
        p = p.normalized();
        d *= pprime.y() / d.y();
        rs.y() = 0;
        rs = rs.normalized();
        float xuprime = Vector3f::dot(pprime, p);
        float drprime = Vector3f::dot(d, rs);
        return drprime - xuprime;
    }

    // p与y轴的距离
    float distFromYAxis(Vector3f p) {
        return sqrt(p.x() * p.x() + p.z() * p.z());
    }

    // 参数曲面上，v的偏导
    Vector3f pointAtv(float u, float v) {
        Vector3f point = pointAt(u, v);
        return Vector3f::cross(Vector3f(0, 1.0, 0), point);
    }

    // 参数曲面上，u的偏导
    Vector3f pointAtu(float u, float v) {
        Vector3f pu = PDeriv(u);
        Matrix4f ry = Matrix4f::rotateY(v);
        return transformDirection(ry, pu);
    }


    // 参数曲面上，u，v对应的点
    Vector3f pointAt(float u, float v) {
        Vector3f p = P(u);
        Matrix4f ry = Matrix4f::rotateY(v);
        return transformPoint(ry, p);
    }

    // 参数曲线的位置
    Vector3f P(float t) {
        Vector3f p (0, 0, 0);
        vector<Vector3f>& controls = pCurve->getControls();
        int n = controls.size() - 1;
        for (int i = 0; i <= n; i++) {
            p += pCurve->B(t, i, n) * controls[i];
        }
        return p;
    }

    // 参数曲线的位置的倒数
    Vector3f PDeriv(float t) {
        Vector3f p (0, 0, 0);
        vector<Vector3f>& controls = pCurve->getControls();
        int n = controls.size() - 1;
        for (int i = 0; i <= n; i++) {
            p += pCurve->BDeriv(t, i, n) * controls[i];
        }
        return p;
    }
};

#endif //REVSURFACE_HPP
