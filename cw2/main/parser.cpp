#include "parser.h"
#include <stdexcept>
#include "core/Scene.h"
#include "shapes/Sphere.h"

#define GETSTR_TO_STD(X) std::string(X.GetString(), X.GetStringLength())

namespace rt {

Parser::Parser() {}
Parser::~Parser() {}

Scene* Parser::ParseScene(rapidjson::Value& scene) {
    auto s = new Scene();

    for (auto& obj : scene.GetObject()) {
        const auto& key = obj.name;

        std::printf("Parsing scene.%s\n", GETSTR_TO_STD(key).data());
        if (key == "shapes") {
            ParseShapes(obj.value);
        }
    }

    return s;
}

std::vector<Shape*>* Parser::ParseShapes(rapidjson::Value& value) {
    auto shapes = new std::vector<Shape*>;
    for (auto& val : value.GetArray()) {
        auto shape = ParseShape(val);
        if (shape) {
            shapes->push_back(shape);
        }
    }
    return shapes;
}

Shape* Parser::ParseShape(rapidjson::Value& value) {
    Shape* shape = nullptr;
    const auto& d = value.GetObject();

    if (!d.HasMember("type")) {
        throw std::invalid_argument("shape missing type");
    }

    const auto& type = d["type"];
    if (type == "sphere") {
        auto center = ParseVec3f(d["center"]);
        auto radius = d["radius"].GetFloat();
        shape = new Sphere(center, radius);
    } else {
        throw std::invalid_argument("unknown type: " + GETSTR_TO_STD(type));
    }

    return shape;
}

Vec3f Parser::ParseVec3f(rapidjson::Value& value) {
    uint field = 0;
    auto vec = Vec3f();
    for (auto& f : value.GetArray()) {
        vec[field] = f.GetFloat();
        field++;
    }
    return vec;
}

}  // namespace rt