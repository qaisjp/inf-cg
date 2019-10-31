#pragma once

#include "rapidjson/document.h"


namespace rt {

class Scene;

class Parser {
    public:
        Parser();
        ~Parser();

        static Scene* ParseScene(rapidjson::Value& scene);
};

} // namespace rt
