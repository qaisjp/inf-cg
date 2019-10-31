#pragma once

#include <vector>
#include "math/geometry.h"
#include "rapidjson/document.h"

namespace rt {

class LightSource;
class Material;
class PointLight;
class Scene;
class Shape;
class Sphere;

class Parser {
   public:
    Parser();
    ~Parser();

    static Scene* ParseScene(rapidjson::Value& scene);

    template <class T>
    static std::vector<T*>* ParseMany(rapidjson::Value& value);

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
