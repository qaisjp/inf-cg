#pragma once

#include <vector>
#include "math/geometry.h"
#include "rapidjson/document.h"

namespace rt {

class Scene;
class Camera;
class Pinhole;
class Shape;
class Sphere;
class LightSource;
class PointLight;
class Material;

class Parser {
   public:
    Parser();
    ~Parser();

    template <class T>
    static std::vector<T*>* ParseMany(rapidjson::Value& value);

    // Scene
    static void Parse(Scene*& ret, rapidjson::Value& value);

    // Cameras
    static void Parse(Camera*& ret, rapidjson::Value& value);
    static void Parse(Pinhole*& cam, rapidjson::Value& d);

    // Shapes
    static void Parse(Shape*& ret, rapidjson::Value& value);
    static void Parse(Sphere*& sphere, rapidjson::Value& d);

    // LightSources
    static void Parse(LightSource*& light, rapidjson::Value& value);
    static void Parse(PointLight*& light, rapidjson::Value& d);

    // Materials
    static void Parse(Material*& mat, rapidjson::Value& value);

    // Geometry
    static Vec3f ParseVec3f(rapidjson::Value& value);
};

}  // namespace rt
