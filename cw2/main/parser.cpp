#include "parser.h"
#include "core/Scene.h"

#define GETSTR_TO_STD(X) std::string(X.GetString(), X.GetStringLength())

namespace rt {

Parser::Parser() {}
Parser::~Parser() {}

Scene* Parser::ParseScene(rapidjson::Value& scene) {
    auto s = new Scene();


    for (auto &obj : scene.GetObject()) {
        auto key = GETSTR_TO_STD(obj.name);

        std::printf("%s ", key.data());
        if (key == "shapes") {
        }
    }

    return s;
}

}