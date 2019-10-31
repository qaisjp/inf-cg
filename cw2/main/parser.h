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
        static std::vector<Shape*>* ParseShapes(rapidjson::Value& value);
};

} // namespace rt
