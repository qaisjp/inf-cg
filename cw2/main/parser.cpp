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
            ParseShapes(obj.value);
        }
    }
    std::printf("\n");

    return s;
}

std::vector<Shape*>* Parser::ParseShapes(rapidjson::Value& value) {
    for (auto& val : value.GetArray()) {
        auto shape = val.GetObject();

        // auto type = GETSTR_TO_STD(shape["type"]); // std::string(shape.name.GetString(), shape.name.GetStringLength());
        // std::printf("%s \n", type.data());
    }

    return nullptr;
}

}