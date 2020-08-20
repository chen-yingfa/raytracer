#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "hit.hpp"
#include "utils.hpp"
#include "texture.hpp"
#include <iostream>
#include <algorithm>

// 虚类
class Material {
public:
    Material() {}
    Material(Texture* _t) : texture(_t) {}
    virtual ~Material() = default;

    virtual Vector3f getEmitColor(float u, float v, const Vector3f& pos) const {
        return Vector3f(0, 0, 0);                   // 不会触发
    }
    
    virtual bool scatter(const Ray& ray, const Hit& hit, Vector3f& attentuation, Ray& scattered) const = 0;
protected:
    Texture* texture;
};

class Lambert : public Material {
public:
    Lambert(Texture* _t) : Material(_t) {}

    virtual bool scatter(const Ray& ray, const Hit& hit, Vector3f& color, Ray& scattered) const {
        // std::cout << "scatter on lambert\n";
        Vector3f scatterDir = hit.getNormal() + Utils::randomUnitVec3();
        scattered = Ray(hit.getPos(), scatterDir.normalized());
        color = texture->getColor(hit.getU(), hit.getV(), hit.getPos());
        return true;
    }
}; 

// shiny
class Metal : public Material {
public:
    Metal (Texture* _t, float fuzz) : Material(_t), fuzziness(fuzz) {}

    virtual bool scatter(const Ray& ray, const Hit& hit, Vector3f& color, Ray& scattered) const {
        Vector3f reflected = Utils::reflect(ray.getDirection(), hit.getNormal());
        Vector3f rayDir = reflected + fuzziness * Utils::randomInUnitSphere();
        scattered = Ray(hit.getPos(), rayDir.normalized());
        color = texture->getColor(hit.getU(), hit.getV(), hit.getPos());
        return (Vector3f::dot(scattered.getDirection(), hit.getNormal()) > 0);
    }
protected:
    // Texture* texture;
    float fuzziness;
};

// glass, water, diamond
class Dielectric : public Material {
public:
    Dielectric (Texture* _t, float ri) : Material(_t), refractIdx(ri) {}

    static float schlick(float cos, float refractIdx) {
        float r = (1 - refractIdx) / (1 + refractIdx);
        r = r*r;
        return r + (1-r) * pow((1-cos), 5);
    }

    virtual bool scatter (const Ray& ray, const Hit& hit, Vector3f& color, Ray& scattered) const {
        color = texture->getColor(hit.getU(), hit.getV(), hit.getPos());
        float etaRatio = hit.getIsOuter() ? (1 / refractIdx) : refractIdx;

        Vector3f rayDir = ray.getDirection();

        float cosTheta = fmin(Vector3f::dot(-rayDir, hit.getNormal()), 1.0f);
        float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

        if (etaRatio * sinTheta > 1.0 || Utils::randomFloat() < schlick(cosTheta, etaRatio)) {
            // reflect
            Vector3f reflected = Utils::reflect(rayDir, hit.getNormal().normalized());
            scattered = Ray(hit.getPos(), reflected.normalized());
        } else {
            // refract
            Vector3f refracted = Utils::refract(rayDir, hit.getNormal(), etaRatio);
            scattered = Ray(hit.getPos(), refracted.normalized());
        }
        return true;
    }

protected:
    float refractIdx;
};


// 用于发光，不反射（直接吃掉光线）
class EmissiveMaterial : public Material {
public:
    EmissiveMaterial(Texture* _t) : Material(_t){}

    virtual bool scatter(const Ray& ray, const Hit& hit, Vector3f& color, Ray& scattered) const {
        return false;  // Emissive Material don't scatter 
    }

    virtual Vector3f getEmitColor(float u, float v, const Vector3f& pos) const {
        return texture->getColor(u, v, pos);  
    }
};

#endif // MATERIAL_H
