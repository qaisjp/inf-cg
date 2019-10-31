#pragma once

#include <vector>
#include "math/geometry.h"
#include "rapidjson/document.h"

namespace rt {

class Scene;
class Shape;

class Parser {
   public:
    Parser();
    ~Parser();

    static Scene* ParseScene(rapidjson::Value& scene);

    // Shapes
    static std::vector<Shape*>* ParseShapes(rapidjson::Value& value);
    static Shape* ParseShape(rapidjson::Value& value);
    static Vec3f ParseVec3f(rapidjson::Value& value);
};

}  // namespace rt
