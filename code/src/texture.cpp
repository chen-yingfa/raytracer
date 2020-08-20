#include <iostream>
#include <vecmath.h>
#include "utils.hpp"
#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// 用stb_image载入图片
ImageTexture::ImageTexture(const char* filename) {
    int compsPerPixel = bytesPerPixel;

    img = stbi_load(filename, &width, &height, &compsPerPixel, compsPerPixel);
    if (!img) {
        std::cerr << "Error: Loading image texture from '" << filename << "' failed\n"; 
        width = height = 0;
    }

    bytesPerRow = bytesPerPixel * width;
}

Vector3f ImageTexture::getColor(float u, float v, const Vector3f& pos) const {
    if (!img) return Vector3f::ZERO; // shows black when image is not loaded

    u = Utils::clamp(u, 0.0, 1.0);
    v = 1.0f - Utils::clamp(v, 0.0, 1.0);

    int x = (int) (u * width);
    int y = (int) (v * height);

    // if u = 1 or v = 1, then x = width or y = height
    if (x >= width) x = width - 1;
    if (y >= height) y = height - 1;

    const float m = 1.0f / 255.0f;
    unsigned char* pixel = img + y * bytesPerRow + x * bytesPerPixel;

    return Vector3f(m * pixel[0], m * pixel[1], m * pixel[2]);
}