#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <ctime>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"
#include "utils.hpp"
#include "bvh.hpp"
#include "box.hpp"
#include "sceneGenerator.hpp"
// #include "perlin.hpp"

#include <string>

using namespace std;


const int samplesPerPixel = 1000;   // SSAA
const int maxDepth = 600;           // 光线跟踪深度上限

// 光线跟踪主要递归函数
Vector3f rayTrace(Ray& ray, Object3D* scene, const Vector3f& bgColor, int depth) {
    if (depth <= 0) return Vector3f(0.01, 0.01, 0.01);
    Hit hit;
    // 判断是否和场景有交点，并返回最近交点的信息（hit）
    if (!scene->intersect(ray, hit, 0.0001, INF)) {  // 若无交点，返回背景颜色
        return bgColor;
    }
    Ray scattered;            // 下一条射线
    Vector3f color(0, 0, 0);  // 颜色
    if (!hit.getMaterial()->scatter(ray, hit, color, scattered)) { 
        // 既不反射亦不折射，是Emissive材质
        return hit.getMaterial()->getEmitColor(hit.getU(), hit.getV(), hit.getPos());
    }
    return color * rayTrace(scattered, scene, bgColor, depth-1);  // 递归
}

int main(int argc, char *argv[]) {
    // 处理args
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3) {
        std::cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];  // 无文件格式

    // 解析场景文件（txt）
    cout << "Parsing scene...\n";
    SceneParser sceneParser(inputFile.c_str());
    Camera *cam = sceneParser.getCamera();
    Group* grp = sceneParser.getGroup();
    Image* img = new Image(cam->getWidth(), cam->getHeight());    // 无已渲染图片 
    cout << "Done parsing scene\n";

    // 载入已渲染图片
    int startX = 0;        // 已渲染的x
    // string loadFilename = "output/temp/ppm/" + to_string(startX) + "empty.ppm";
    // Image* img = Image::LoadPPM(loadFilename.c_str());    // 载入
    // int samplesOnStart = 0;    // 已采样次数


    // 程序化修改场景...
    SceneGenerator sceneGen;   // 场景生成器（比较简陋）
    sceneGen.getScene1(grp);   // 一个 Minecraft 场景，小屋子，有矿的洞口
    


    cout << "Camera resolution: " << cam->getWidth() << "x" << cam->getHeight() << "\n";
    cout << "Number of objects in scene: " << grp->getGroupSize() << "\n";
    cout << "Sampling per pixel: " << samplesPerPixel << "\n";
    cout << "Raytracing max bounce: " << maxDepth << "\n";

    // 用于计时
    clock_t startTime = clock();
    float lastTime = 0;

    // 建立BVH树
    cout << "Building BVH Tree for scene...\n";
    BvhNode* bvhRoot = new BvhNode(grp);         //  求交加速：对整个场景的 Group （所有物体）建立 BVH 树
    cout << "Done building BVH Tree\n";

    Vector3f bgColor = Vector3f::ZERO;

    // 遍历像素
    for (int x = startX; x < cam->getWidth(); ++x){    // 左至右
        for (int y = 0; y < cam->getHeight(); ++y){      // 下至上
            Vector3f pixelColor = Vector3f::ZERO;

            // 每像素(x,y)执行多次光线投射
            for (int s = 0; s < samplesPerPixel; s++){
                Vector2f screenPoint(x + Utils::randomFloat(), y + Utils::randomFloat()); // 景深效果
                Ray camRay = cam->generateRay(screenPoint);                               // 光线投射
                pixelColor += rayTrace(camRay, bvhRoot, bgColor, maxDepth);               // 执行光线跟踪
            }

            // 若载入已采样图片
            // Vector3f prevColor = img->GetPixel(x, y);
            // prevColor = prevColor * prevColor;    // undo gamma correction
            // prevColor *= samplesOnStart;          // undo average 
            // pixelColor += prevColor;
            // pixelColor /= samplesPerPixel + samplesOnStart;  // average out samples (SSAA)

            pixelColor /= samplesPerPixel;                     // 取采样颜色平均
            pixelColor = Utils::sqrtVec3(pixelColor);          // 伽马纠正
            img->SetPixel(x, y, pixelColor);

            // 每秒计算并输出用时和预计剩余时间
            float timeElapsed = Utils::getTimeElapsed(startTime);
            if (timeElapsed - lastTime > 1.0f) { 
                lastTime += 1.0f;
                float estTimeLeft = (((float) cam->getWidth() - x) / (x+1)) * timeElapsed;
                printf("[%4d/%4d] ", x, cam->getWidth());          // 输出已处理多少列像素
                printf("Time elapsed: %.2f, Est. time left: %.2f\n", timeElapsed, estTimeLeft);
            }
        }

        // 每10列保存中间图片
        if ((x) % 10 == 0) {
            string fnamebmp = "output/temp/bmp/" + to_string(x) + outputFile + ".bmp";
            string fnameppm = "output/temp/ppm/" + to_string(x) + outputFile + ".ppm";
            std::cout << "Image saved! File name: " << fnamebmp.c_str() << endl;
            std::cout << "Image saved! File name: " << fnameppm.c_str() << endl;
            img->SaveBMP(fnamebmp.c_str());
            img->SavePPM(fnameppm.c_str());
        }
    }

    // 保存最终结果
    string fnamebmp = "output/" + outputFile + ".bmp";
    string fnameppm = "output/" + outputFile + ".ppm";
    img->SaveBMP(fnamebmp.c_str());
    img->SavePPM(fnameppm.c_str());
    std::cout << "Image saved! File name: " << outputFile.c_str() << endl;
    return 0;
}

