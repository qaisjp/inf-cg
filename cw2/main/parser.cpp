#include "parser.h"
#include "core/Scene.h"
#include "shapes/Sphere.h"
#include <stdexcept>

#define GETSTR_TO_STD(X) std::string(X.GetString(), X.GetStringLength())

namespace rt {

Parser::Parser() {}
Parser::~Parser() {}

Scene* Parser::ParseScene(rapidjson::Value& scene) {
    auto s = new Scene();


    for (auto &obj : scene.GetObject()) {
        auto key = GETSTR_TO_STD(obj.name);

        std::printf("%s: ", key.data());
        if (key == "shapes") {

            ParseShapes(obj.value);
        }
        std::printf("\n");
    }

    return s;
}

std::vector<Shape*>* Parser::ParseShapes(rapidjson::Value& value) {
    auto shapes = new std::vector<Shape*>;
    for (auto& val : value.GetArray()) {
        auto shape = ParseShape(val);
        shapes->push_back(shape);
    }
    return shapes;
}

Shape* Parser::ParseShape(rapidjson::Value& value) {
    Shape* shape = nullptr;
    auto d = value.GetObject();
    auto type = GETSTR_TO_STD(d["type"]);

    std::printf("%s ", type.data());
    if (type == "sphere") {
        shape = new Sphere(Vec3f(0,0,0), 0);
    } else {
        throw std::invalid_argument("Unknown type");
    }

    return shape;
}

}