#include "parser.h"
#include "core/Scene.h"

namespace rt {

Parser::Parser() {}
Parser::~Parser() {}

Scene* Parser::ParseScene(rapidjson::Value& scene) {
    auto s = new Scene();


    for (auto &obj : scene.GetObject()) {
        auto& name = obj.name;
        auto key = std::string(name.GetString(), name.GetStringLength());

        std::printf("%s ", key.data());
        if (key == "shapes") {
        }
    }

    return s;
}

}