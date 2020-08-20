#ifndef SCENE_GENERATOR_H
#define SCENE_GENERATOR_H

#include <vector>
#include <vecmath.h>
#include <iostream>
#include "utils.hpp"
// #include "perlin.hpp"
#include "group.hpp"
#include "object3d.hpp"
#include "utils.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "box.hpp"
#include "mesh.hpp"
#include "sphere.hpp"

using namespace std;

// 目前全手动生成，只有两个场景，主要用于生成《我的世界》场景。
// 主要逻辑：
// 场景由一个函数生成，程序化生成一个三维数组，
// 元素为MinecraftBlock枚举类型，最后遍历该数组，
// 给场景的Group添加对应的物体。

enum MinecraftBlock {emptyBlock, grass, dirt, oakPlank, oakLog, stone, lightBlock};

class SceneGenerator {
public:
    Texture* white        = new SolidColor(1, 1, 1);
    Texture* black        = new SolidColor(0.1, 0.1, 0.1);
    Texture* defaultColor = new SolidColor(0.8, 0.8, 0.8);
    Texture* sunsetColor  = new SolidColor(0.9, 0.6, 0.5);
    Texture* red          = new SolidColor(0.9, 0.6, 0.6);
    Texture* green        = new SolidColor(0.6, 0.9, 0.6);
    Texture* blue         = new SolidColor(0.6, 0.6, 0.9);
    Texture* textureDirt      = new ImageTexture("textures/minecraft/dirt.png");
    Texture* textureGrassSide = new ImageTexture("textures/minecraft/grass_side.png");
    Texture* textureGrassTop  = new ImageTexture("textures/minecraft/grass_top.png");
    Texture* textureOakPlank  = new ImageTexture("textures/minecraft/oak_planks.png");
    Texture* textureOakLog    = new ImageTexture("textures/minecraft/oak_log.png");
    Texture* textureOakLogTop = new ImageTexture("textures/minecraft/oak_log_top.png");
    Texture* textureStone     = new ImageTexture("textures/minecraft/stone.png");
    Lambert* matDirt       = new Lambert(textureDirt);
    Lambert* matGrassSide  = new Lambert(textureGrassSide);
    Lambert* matGrassTop   = new Lambert(textureGrassTop);
    Lambert* matOakPlank   = new Lambert(textureOakPlank);
    Lambert* matOakLog     = new Lambert(textureOakLog);
    Lambert* matOakLogTop  = new Lambert(textureOakLogTop);
    Lambert* matStone      = new Lambert(textureStone);
    Lambert* defaultMat    = new Lambert(defaultColor);
    Lambert* whiteMat      = new Lambert(white);
    Lambert* blackMat      = new Lambert(black);
    Lambert* redMat        = new Lambert(red);
    Lambert* greenMat      = new Lambert(green);
    Lambert* blueMat       = new Lambert(blue);

    Vector3f boxSize = Vector3f(1, 1, 1);
    Vector3f minPos = Vector3f(-6, -4, -6);   // 场景中坐标最小的位置
    Vector3f nBlocks = Vector3f(12, 12, 12);  // 不同方向的方体个数

    vector<vector<vector<enum MinecraftBlock>>> minecraftBlocks;
public:
    SceneGenerator () {}

    // 用于获得材质

    void addSkySphere(Group* grp) {
        Material* skyMat = new EmissiveMaterial(new ImageTexture("textures/skymap.jpg"));
        Sphere* s = new Sphere(Vector3f(0,0,0), 100, skyMat);  // sky sphere
        grp->addObject(s);
    }

    void addNightSphere(Group* grp) {
        Material* skyMat = new EmissiveMaterial(new ImageTexture("textures/space.png"));
        Sphere* s = new Sphere(Vector3f(0,0,0), 100, skyMat);  // sky sphere
        grp->addObject(s);
    }

    inline Material* getLightMat(float x) {
        return new EmissiveMaterial(new SolidColor(Vector3f(x, x, x)));
    }

    inline Material* getLightMat(Vector3f color) {
        return new EmissiveMaterial(new SolidColor(color));
    }
    
    // 返回一些《我的世界》的方体

    inline Box* getBoxLight(Vector3f pos, Vector3f size, Vector3f color) {
        Box* b = new Box(pos, pos + size, getLightMat(color));
        return b;
    }

    inline Box* getBoxGrass(Vector3f pos, Vector3f size) {
        Box* b = new Box(pos, pos + size, matGrassSide);
        b->setBottom(matDirt);
        b->setTop(matGrassTop);
        return b;
    }

    inline Box* getBoxDirt(Vector3f pos, Vector3f size) {
        return new Box(pos, pos+size, matDirt);
    }

    inline Box* getBoxOakPlank(Vector3f pos, Vector3f size) {
        Box* b = new Box(pos, pos+size, matOakPlank);
        return b;
    }

    inline Box* getBoxOakLog(Vector3f pos, Vector3f size) {
        Box* b = new Box(pos, pos+size, matOakLog);
        b->setTop(matOakLogTop);
        b->setBottom(matOakLogTop);
        return b;
    }

    inline Box* getBoxStone(Vector3f pos, Vector3f size) {
        Box* b = new Box(pos, pos+size, matStone);
        return b;
    }

    // 获得指定类型的《我的世界》方体

    Box* getBlock(enum MinecraftBlock block, Vector3f pos) {
        return getBlock(block, pos, boxSize);
    }

    Box* getBlock(enum MinecraftBlock block, Vector3f pos, Vector3f size) {
        if (block == grass) {
            return getBoxGrass(pos, size);
        } else if (block == dirt) {
            return getBoxDirt(pos, size);
        } else if (block == oakPlank) {
            return getBoxOakPlank(pos, size);
        } else if (block == oakLog) {
            return getBoxOakLog(pos, size);
        } else if (block == stone) {
            return getBoxStone(pos, size);
        } else {
            return nullptr;
        }
    }

    // 根据给定二维向量，添加地形

    void addMinecraftTerrain(vector<vector<int>> terrain, Vector3f minPos) {
        // fill blocks according to 2D array landscape
        for (int x = 0; x < terrain.size(); x++){
            for (int z = 0; z < terrain[x].size(); z++) {
                // add dirt blocks
                int y = 0;
                for (; y < terrain[x][z] - 1; y++) {
                    // just add internal blocks, will not add them to object group in the end
                    minecraftBlocks[x][y][z] = dirt;
                }
                // add grass block on top of each column
                minecraftBlocks[x][y][z] = grass;
            }
        }
    }

    // 添加形成一个矩形的方体

    void addBlockPlaneX(int x, int miny, int maxy, int minz, int maxz, enum MinecraftBlock block) {
        for (int y = miny; y < maxy; y++){
            for (int z = minz; z < maxz; z++){
                minecraftBlocks[x][y][z] = block;
            }
        }
    } 

    void addBlockPlaneY(int y, int minx, int maxx, int minz, int maxz, enum MinecraftBlock block) {
        for (int x = minx; x < maxx; x++){
            for (int z = minz; z < maxz; z++){
                minecraftBlocks[x][y][z] = block;
            }
        }
    }

    void addBlockPlaneZ(int z, int minx, int maxx, int miny, int maxy, enum MinecraftBlock block) {
        for (int x = minx; x < maxx; x++){
            for (int y = miny; y < maxy; y++){
                minecraftBlocks[x][y][z] = block;
            }
        }
    }

    // 添加一个

    void addBlockBox(Vector3f mn, Vector3f mx, enum MinecraftBlock block) {
        addBlockPlaneX(mn.x(), mn.y(), mx.y(), mn.z(), mx.z(), block);
        addBlockPlaneX(mx.x()-1, mn.y(), mx.y(), mn.z(), mx.z(), block);
        addBlockPlaneY(mn.y(), mn.x(), mx.x(), mn.z(), mx.z(), block);
        addBlockPlaneY(mx.y()-1, mn.x(), mx.x(), mn.z(), mx.z(), block);
        addBlockPlaneZ(mn.z(), mn.x(), mx.x(), mn.y(), mx.y(), block);
        addBlockPlaneZ(mx.z()-1, mn.x(), mx.x(), mn.y(), mx.y(), block);
    }

    void addBlock(Vector3f pos, enum MinecraftBlock block) {
        minecraftBlocks[pos.x()][pos.y()][pos.z()] = block;
    }

    // 一个小屋子，有些灯光，两只兔子，三个球。
    void getScene1(Group* grp) {
        Material* glass = new Dielectric(white, 1.5);
        Material* clearMetal = new Metal(white, 0);
        Material* fuzzyMetal = new Metal(white, 0.2);
        int gndY = -minPos.y();
        minecraftBlocks.resize(nBlocks.x());
        for (int i = 0; i < nBlocks.x(); i++) {
            minecraftBlocks[i].resize(nBlocks.y());
            for (int j = 0; j < nBlocks.y(); j++){
                minecraftBlocks[i][j].resize(nBlocks.z());
                for (int k = 0; k < nBlocks.z(); k++) {
                    minecraftBlocks[i][j][k] = emptyBlock;
                }
            }
        }
        vector<vector<int>> terrain(nBlocks.x(), vector<int>(nBlocks.z()));

        // manually generate terrain
        for (int i = 0; i < terrain.size(); i++){
            for (int j = 0; j < terrain[i].size(); j++) {
                int h = gndY;
                if (i < 3) h += 2;
                if (i < 1) h += 1;
                // cout << h;
                terrain[i][j] = h;
            }
            // cout << endl;
        }

        addMinecraftTerrain(terrain, minPos);

        // add house
        Vector3f tmp;
        Vector3f ones(1, 1, 1);
        Vector3f vx(1, 0, 0);
        Vector3f vy(0, 1, 0);
        Vector3f vz(0, 0, 1);
        Vector3f housePos(3, gndY, 2);
        Vector3f houseSize(5, 3, 6);
        Vector3f houseLenX(houseSize.x(), 0, 0);
        Vector3f houseLenY(0, houseSize.y(), 0);
        Vector3f houseLenZ(0, 0, houseSize.z());
        Vector3f pillarSize = houseLenY + ones;
        Vector3f cornerFR = housePos + houseLenX;
        Vector3f cornerBR = housePos + houseLenX + houseLenZ;
        Vector3f cornerBL = housePos + houseLenZ;
        addBlockBox(housePos, housePos + houseLenX + houseLenY + ones, oakPlank);  // back wall
        addBlockBox(housePos+vx, housePos+vx + 2*vz + houseLenY + ones, oakPlank); // left Wall
        addBlockBox(housePos + houseLenX, housePos + houseSize + ones, oakPlank);  // right Wall
        addBlockBox(housePos + houseLenY, housePos + houseSize + vy, oakPlank);  // ceiling
        addBlockBox(housePos - ones, housePos + houseLenX + houseLenZ, oakPlank);   // floor

        tmp = housePos + houseLenX + vy + 2*vz;
        addBlockBox(tmp, tmp + 2 * vz + ones, emptyBlock); // add window to right wall

        addBlockBox(cornerFR, cornerFR + pillarSize, oakLog);    // front right pillar
        // addBlockBox(housePos, housePos + pillarSize, oakLog);    // front left pillar
        addBlockBox(cornerBR, cornerBR + pillarSize, oakLog);    // back right pillar
        addBlockBox(cornerBL, cornerBL + pillarSize, oakLog);    // back left pillar
        tmp = housePos + houseLenY;
        // addBlockBox(tmp, tmp + houseLenX + ones, oakLog);     // front wall upper rim
        tmp = housePos + houseLenX + houseLenY;
        addBlockBox(tmp, tmp + houseLenZ + ones, oakLog);     // right wall upper rim
        tmp = housePos + houseLenZ + houseLenY;
        addBlockBox(tmp, tmp + houseLenX + ones, oakLog);     // back wall upper rim
        tmp = housePos + houseLenY;
        addBlockBox(tmp, tmp + houseLenZ + ones, oakLog);     // left wall upper rim
        

        // add cave opening
        tmp = cornerBL - 1*vz - 2*vx;
        addBlockBox(tmp, tmp + 2*vy + 2*vx + ones, stone);   // back
        tmp = cornerBL - 3*vz - 2*vx; 
        addBlockBox(tmp, tmp + 2*vy + 2*vx + ones, stone);     // front
        tmp = cornerBL + Vector3f(-2, 2, -2);
        addBlockBox(tmp, tmp + 2*vx + ones, stone);          // middle
        tmp = cornerBL + Vector3f(-1, 1, -2);
        addBlock(tmp, stone);
        tmp -= vy;
        addBlock(tmp, stone);
        tmp = cornerBL + Vector3f(-2, -1, -2);
        addBlockBox(tmp, tmp + 2*vx + 1*vy + ones, emptyBlock);


        // add lights
        tmp = cornerBL + Vector3f(-2, 0, -2);
        // Vector3f lightColor(10, 6, 5);
        Vector3f lightColor(16, 12, 10);
        grp->addObject(getBoxLight(gridToPos(tmp), boxSize, lightColor));    // adds light coming from cave
        Material* roofLight = getLightMat(16); // 屋顶的一个平面灯
        // grp->addObject(new RectY(0, 1, 0, 1, 2.99, roofLight));

        
        // other objects

        // spheres
        float r0 = 0.3, r1 = 0.3, r2 = 0.3;
        tmp = gridToPos(cornerBL) + 1.3*vx + r0*vy - 1.5*vz;
        grp->addObject(new Sphere(tmp, r0, glass));
        tmp = gridToPos(cornerBL) + 2*vx + r1*vy - 1.5*vz;
        grp->addObject(new Sphere(tmp, r1, clearMetal));
        tmp = gridToPos(cornerBL) + 2.7*vx + r2*vy - 1.5*vz;
        grp->addObject(new Sphere(tmp, r2, fuzzyMetal));

        
        Material* bunnyMat = new Lambert(new SolidColor(Vector3f(0.6, 0.6, 0.6)));
        char bunnyFile[] = "mesh/bunny_1k.obj";
        Mesh* meshBunnyMetal = new Mesh(bunnyFile, fuzzyMetal);
        Mesh* meshBunnyLambert = new Mesh(bunnyFile, bunnyMat);

        // transform metal bunny
        float scale = 3;
        float offY = 0.07 * scale;
        tmp = cornerFR + 2*vz;
        tmp = gridToPos(tmp) - 1.6*vx - offY*vy + 0.4 * vz;
        Matrix4f m = Matrix4f::identity();
        m = m * Matrix4f::translation(tmp);
        m = m * Matrix4f::uniformScaling(scale);
        Transform* transformBunny0 = new Transform(m, meshBunnyMetal);
        
        tmp = cornerBR + vz;
        tmp = gridToPos(tmp) - 0.6*vx - offY*vy;
        m = Matrix4f::identity();
        m = m * Matrix4f::translation(tmp);
        m = m * Matrix4f::uniformScaling(scale);
        Transform* transformBunny1 = new Transform(m, meshBunnyMetal);

        // add objects to group

        grp->addObject(transformBunny0);
        grp->addObject(transformBunny1);
        addSkySphere(grp);  // skybox, daylight
        addMinecraftBlocks(grp);
    }
    

    // 简单《我的世界》场景
    Group* getMinecraftScene() {
        Group* grp = new Group(0);
        Texture* textureDirt = new ImageTexture("textures/minecraft/dirt.png");
        Texture* textureGrassSide = new ImageTexture("textures/minecraft/grass_side.png");
        Texture* textureGrassTop = new ImageTexture("textures/minecraft/grass_top.png");
        Lambert* matDirt = new Lambert(textureDirt);
        Lambert* matGrassSide = new Lambert(textureGrassSide);
        Lambert* matGrassTop = new Lambert(textureGrassTop);
        Vector3f boxSize(1, 1, 1);
        float yMin = 0;
        float xMin = -8, zMin = -8;
        int numBlockX = 16, numBlockZ = 16;
        Vector3f minPos(xMin, yMin, zMin);
        vector<vector<int>> landscape(numBlockX, vector<int>(numBlockZ));
        
        // // generate landscape with perlin noise
        // Perlin perlin;
        // for (int i = 0; i < numBlockX; i++){
        //     for (int j = 0; j < numBlockZ; j++) {
        //         float x = 5 * ((float) i ) / numBlockX;
        //         float z = 5 * ((float) j ) / numBlockZ;
        //         Vector3f p = (x, z, 0);
        //         float n = perlin.noise(p);
        //         cout << perlin.noise(p) << " ";
        //         landscape[i][j] = 10 * perlin.noise(p);
        //         cout << landscape[i][j];
        //     }
        //     cout << endl;
        // }


        // manually generate terrain
        for (int i = 0; i < numBlockX; i++){
            for (int j = 0; j < numBlockZ; j++) {
                float ii = ((float) i) / numBlockX;
                float jj = ((float) j) / numBlockZ;
                
                int height = (int) (abs(ii-0.5)*abs(ii-0.5)*20 + abs(jj-0.5)*abs(jj-0.5)*20);
                cout << height;
                landscape[i][j] = height+1;
            }
            cout << endl;
        }

        // fill blocks according to 2D array landscape
        for (int i = 0; i < numBlockX; i++){
            for (int j = 0; j < numBlockZ; j++) {
                // add dirt block
                int k = 0;
                for (; k < landscape[i][j] - 1; k++) {
                    // don't add internal blocks, won't be visible anyways
                    if (1 <= i && i <= numBlockX-2 && 1 <= j && j <= numBlockZ-2 &&
                        k < landscape[i+1][j] && k < landscape[i-1][j] && 
                        k < landscape[i][j+1] && k < landscape[i][j-1]) {
                        continue;
                    }
                    Vector3f t(i, k, j);
                    Vector3f pos = gridToPos(t);
                    Vector3f size = pos + boxSize;
                    Box* box = new Box(pos, size, matDirt);
                    grp->addObject(box);
                }
                // add grass block
                Vector3f t(i, k, j);
                Vector3f pos = gridToPos(t);
                Vector3f size = pos + boxSize;
                Box* box = new Box(pos, size, matGrassSide);
                box->setTop(matGrassTop);
                grp->addObject(box);
            }
        }


        // // fill ground with dirt
        // for (float x = xMin; x < xMax; x += boxSize.x()) {
        //     for (float z = zMin; z < zMax; z += boxSize.z()) {
        //         Vector3f pos(x, yMin, z);
        //         Vector3f size = pos + boxSize;
        //         Box* box = new Box(pos, size, matGrassSide);
        //         box->setTop(matGrassTop);
        //         grp->addObject(box);
                
        //     }
        // }

        // // generate variable height landscape
        // for (float x = xMin; x < xMax; x += boxSize.x()){
        //     for (float z = zMin; z < zMax; z += boxSize.z()) {
        //         float maxY = boxSize.y() * Utils::randomInt(1, 3);
        //         for (int y = 1; y < maxY; y += boxSize.y()) {
        //             Vector3f pos(x, y, z);
        //             Vector3f size = pos + boxSize;
        //             Box* box = new Box(pos, size, matGrassSide);
        //             box->setTop(matGrassTop);
        //             grp->addObject(box);
        //         }

        //     }
        // }
        return grp;
    
    }
    
    // Utils

    void addMinecraftBlocks(Group* grp) {
        int nx = nBlocks.x();
        int ny = nBlocks.y();
        int nz = nBlocks.z();
        for (int x = 0; x < nx; x++) {
            for (int y = 0; y < ny; y++) {
                for (int z = 0; z < nz; z++) {
                    Vector3f g(x, y, z);
                    if (isHidden(g)) continue;
                    if (minecraftBlocks[x][y][z] == emptyBlock) continue;
                    Vector3f p = gridToPos(g);
                    grp->addObject(getBlock(minecraftBlocks[x][y][z], p));
                }
            }
        }
    }


    inline bool isHidden(Vector3f p) {
        int x = p.x(), y = p.y(), z = p.z();

        // on the sides of the scene
        if (x == 0 || x == nBlocks.x() - 1) return false;
        if (y == 0 || y == nBlocks.y() - 1) return false;
        if (z == 0 || z == nBlocks.z() - 1) return false;

        // has one adjacent empty block
        if (minecraftBlocks[x-1][y][z] == emptyBlock ||
            minecraftBlocks[x+1][y][z] == emptyBlock ||
            minecraftBlocks[x][y-1][z] == emptyBlock ||
            minecraftBlocks[x][y+1][z] == emptyBlock ||
            minecraftBlocks[x][y][z-1] == emptyBlock ||
            minecraftBlocks[x][y][z+1] == emptyBlock) return false;
        return true;
    }

    inline Vector3f gridToPos(Vector3f g) {
        return minPos + g * boxSize;
    }
};

#endif // SCENE_GENERATOR_H