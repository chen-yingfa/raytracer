#ifndef UTILS_H
#define UTILS_H

#include <random>
#include <ctime>

// constants
const double PI = 3.14159265358979323846;
const double INF = std::numeric_limits<float>::infinity();

class Utils {
public:
    static inline float clamp(float x, float a, float b) {
        if (x < a) return a;
        if (x > b) return b;
        return x;
    }

    static inline float getTimeElapsed(clock_t startTime) {
        clock_t t = clock() - startTime;
        return ((float) t) / CLOCKS_PER_SEC;
    }

    static inline int randomInt(int mn, int mx) {
        return (int) randomFloat(mn, mx);
    }

    static inline float randomFloat() {
        static std::uniform_real_distribution<float> distri(0.0, 1.0);
        static std::mt19937 generator;
        return distri(generator);
    }

    static inline float randomFloat(float mn, float mx) {
        return mn + (mx - mn) * randomFloat();
    }

    static inline Vector3f randomVec3() {
        return Vector3f(randomFloat(), randomFloat(), randomFloat());
    }

    static inline Vector3f randomVec3(float mn, float mx) {
        return Vector3f(randomFloat(mn, mx), randomFloat(mn, mx), randomFloat(mn, mx));
    }

    static inline Vector2f randomInUnitDisk(){
        Vector2f r(randomFloat(), randomFloat());
        while (Vector2f::dot(r, r) >= 1)
            r = Vector2f(randomFloat(), randomFloat());
        return r;
    }

    static Vector3f randomInUnitSphere() {   // get point in unit sphere by rejection method
        Vector3f r = randomVec3(-1, 1);
        while (Vector3f::dot(r, r) >= 1)
            r = randomVec3(-1, 1);
        return r;
    }

    static Vector3f randomUnitVec3() {
        float a = randomFloat(0, 2*PI);
        float z = randomFloat(-1, 1);
        float r = sqrt(1 - z * z);
        return Vector3f(r * cos(a), r * sin(a), z);
    }

    static Vector3f randomInHemisphere(const Vector3f normal) {
        Vector3f inUnitSphere = randomInUnitSphere();
        if (Vector3f::dot(inUnitSphere, normal) > 0.0)
            return inUnitSphere;
        else
            return -inUnitSphere;
    }

    static inline Vector3f sqrtVec3(Vector3f& v) {
        return Vector3f(sqrt(v.x()), sqrt(v.y()), sqrt(v.z()));
    }

    static Vector3f reflect(const Vector3f& v, const Vector3f& normal) {
        return v - 2 * Vector3f::dot(v, normal) * normal;
    }

    static Vector3f refract(const Vector3f& v, const Vector3f& normal, float etaConst) {
        float cosTheta = Vector3f::dot(-v, normal);
        Vector3f rParallell = etaConst * (v + cosTheta * normal);
        Vector3f rPerpendicular = -sqrt(1.0 - Vector3f::dot(rParallell, rParallell)) * normal;
        return rParallell + rPerpendicular;
    }

    static inline float snapGrid(float x, float gridSize) {
        return gridSize * (((int) x) / gridSize);
    }
};

#endif