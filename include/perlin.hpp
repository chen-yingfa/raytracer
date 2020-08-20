#ifndef PERLIN_H
#define PERLIN_H

#include <cmath>
#include <vecmath.h>
#include "utils.hpp"

// Incomplete.. has bugs, does not generate anything
// TODO: complete this Perlin class

class Perlin {
    public:
        Perlin() {
            randfloats = new float[pointCnt];
            for (int i = 0; i < pointCnt; i++) {
                randfloats[i] = Utils::randomFloat();
            }

            permX = perlinGenPerm();
            permY = perlinGenPerm();
            permZ = perlinGenPerm();
        }

        ~Perlin() {
            delete[] randfloats;
            delete[] permX;
            delete[] permY;
            delete[] permZ;
        }

        float noise(const Vector3f& p) const {
            float u = p.x() - floor(p.x());
            float v = p.y() - floor(p.y());
            float w = p.z() - floor(p.z());

            int i = (int) (p.x());
            int j = (int) (p.y());
            int k = (int) (p.z());
            double c[2][2][2];

            for (int di = 0; di < 2; di++)
                for (int dj = 0; dj < 2; dj++)
                    for (int dk = 0; dk < 2; dk++)
                        c[di][dj][dk] = randfloats[
                            permX[(i+di) & 255] ^
                            permY[(j+dj) & 255] ^
                            permZ[(k+dk) & 255]
                        ];

            auto sum = 0.0;
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    for (int k = 0; k < 2; k++)
                        sum += (i*u + (1-i)*(1-u)) *
                               (j*v + (1-j)*(1-v)) *
                               (k*w + (1-k)*(1-w)) * c[i][j][k];

            return sum;
        }

        float noise2d(float x, float y) {
            x %= 256;
            y %= 256;
            int xi = (int) x & 255;
            int yi = (int) y & 255;
            float xf = x - (int) x;
            float yf = y - (int) y;

            Vector2f TR = Vector2f(u-1, v-1);
            Vector2f TL = Vector2f(u, v-1);
            Vector2f BR = Vector2f(u-1, v);
            Vector2f BL = Vector2f(u, v);

            int* P = perlinGenPerm();

            int vTR = P[P[X+1]+Y+1];
            int vTL = P[P[X]+Y+1];
            int vBR = P[P[X+1]+Y];
            int vBL = P[P[X]+Y];

            for (int di = 0; di < 2; di++)
                for (int dj = 0; dj < 2; dj++)
                    for (int dk = 0; dk < 2; dk++)
                        c[di][dj][dk] = randfloats[
                            permX[(i+di) & 255] ^
                            permY[(j+dj) & 255] ^
                            permZ[(k+dk) & 255]
                        ];

            auto sum = 0.0;
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    for (int k = 0; k < 2; k++)
                        sum += (i*u + (1-i)*(1-u)) *
                               (j*v + (1-j)*(1-v)) *
                               (k*w + (1-k)*(1-w)) * c[i][j][k];

            return sum;
        }

        float face(float t) {
            return t * t * t * (t * (t * 6 - 15) + 10);
        }

    private:
        static const int pointCnt = 256;
        float* randfloats;
        int* permX;
        int* permY;
        int* permZ;

        Vector2f getConstVec(int x) {
            int h = x % 4;
            if (h == 0)
                return Vector2f(1, 1);
            else if (h == 1)
                return Vector2f(-1, 1);
            else if (h == 2)
                return Vector2f(-1, -1);
            else 
                return Vector2f(1, -1);
        }

        static int* perlinGenPerm() {
            int* p = new int[pointCnt];
            for (int i = 0; i < Perlin::pointCnt; i++)
                p[i] = i;

            shuffle(p, pointCnt);
            return p;
        }

        static void shuffle(int* p, int n) {
            for (int i = n-1; i > 0; i--) {
                int target = Utils::randomInt(0, i);
                int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
        }
};

#endif // PERLIN_H