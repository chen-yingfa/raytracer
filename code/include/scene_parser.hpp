#ifndef SCENE_PARSER_H
#define SCENE_PARSER_H

#include <cassert>
#include <vecmath.h>
#include <vector>

#include "scene_parser.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "object3d.hpp"
#include "group.hpp"
#include "mesh.hpp"
#include "sphere.hpp"
#include "rectangle.hpp"
#include "plane.hpp"
#include "triangle.hpp"
#include "transform.hpp"
#include "curve.hpp"
#include "revsurface.hpp"

class Camera;
class Light;
class Material;
class Object3D;
class Group;
class Sphere;
class Plane;
class Triangle;
class Transform;
class Mesh;
class Curve;
class RevSurface;

#define MAX_PARSER_TOKEN_LENGTH 1024

class SceneParser {
public:

    SceneParser() = delete;
    SceneParser(const char *filename);

    ~SceneParser();

    Camera *getCamera() const { return camera; }

    Vector3f getBackgroundColor() const { return background_color; }

    int getNumLights() const { return lights.size(); }

    Light *getLight(int i) const {
        assert(i >= 0 && i < lights.size());
        return lights[i];
    }

    int getNumTexture() const { return textures.size(); }

    Texture *getTexture(int i) const {
        assert (0 <= i && i < textures.size());
        return textures[i];
    }

    int getNumMaterials() const { return materials.size(); }

    Material *getMaterial(int i) const {
        assert(i >= 0 && i < materials.size());
        return materials[i];
    }

    void addMaterial(Material* m) {
        materials.push_back(m);
    }

    Group *getGroup() const { return group; }

private:

    void parseFile();
    void parsePerspectiveCamera();
    void parseBackground();
    void parseLights();
    Light *parsePointLight();
    Light *parseDirectionalLight();
    void parseTextures();
    CheckerTexture *parseCheckerTexture();
    ImageTexture *parseImageTexture();

    void parseMaterials();
    Lambert *parseLambert();
    Metal *parseMetal();
    Dielectric* parseDielectric();
    EmissiveMaterial *parseEmissiveMaterial();

    Object3D *parseObject(char token[MAX_PARSER_TOKEN_LENGTH]);
    Group *parseGroup();
    Sphere *parseSphere();
    Plane *parsePlane();
    Triangle *parseTriangle();
    Mesh *parseTriangleMesh();
    Transform *parseTransform();
    Curve *parseBezierCurve();
    Curve *parseBsplineCurve();
    RevSurface *parseRevSurface();

    int getToken(char token[MAX_PARSER_TOKEN_LENGTH]);

    Vector3f readVector3f();

    float readFloat();
    int readInt();

    FILE *file;
    Camera *camera;
    Vector3f background_color;
    std::vector<Light*> lights;
    std::vector<Material*> materials;
    std::vector<Texture*> textures;
    Material *current_material;
    Group *group;
};

#endif // SCENE_PARSER_H
