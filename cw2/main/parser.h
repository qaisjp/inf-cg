#pragma once

#include "rapidjson/document.h"
#include <vector>

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
};

} // namespace rt
