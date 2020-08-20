#ifndef TEXTURE_H
#define TEXTURE_H

#include <vecmath.h>

class Texture {
public:
    Texture(){}
    virtual Vector3f getColor(float u, float v, const Vector3f& pos) const = 0;
};

class SolidColor : public Texture {
public:
    SolidColor(){}
    SolidColor(Vector3f c) : color(c){}
    SolidColor(float r, float g, float b) : color(Vector3f(r, g, b)){}

    virtual Vector3f getColor(float u, float v, const Vector3f& pos) const {
        return color;
    }
private:
    Vector3f color;
};

class ImageTexture : public Texture {
public:
    static const int bytesPerPixel = 3;

    ImageTexture() : img(nullptr), width(0), height(0), bytesPerRow(0) {}
    ImageTexture(const char* filename);

    // destructor
    ~ImageTexture() {
        delete img;
    }

    virtual Vector3f getColor(float u, float v, const Vector3f& pos) const;

private:
    unsigned char* img;
    int width, height;
    int bytesPerRow;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture(){}
    CheckerTexture(Texture* _t0, Texture* _t1) : texture0(_t0), texture1(_t1) {}

    virtual Vector3f getColor(float u, float v, const Vector3f& pos) const {
        float s = sin(1 * pos.x()) * sin(1 * pos.y()) * sin(1 * pos.z());
        if (s < 0) {
            return texture0->getColor(u, v, pos);
        } else {
            return texture1->getColor(u, v, pos);
        }
    }

    void setTexture0(Texture* t) {texture0 = t;}
    void setTexture1(Texture* t) {texture1 = t;}

private:
    Texture *texture0;
    Texture *texture1;
};

#endif // TEXTURE_H