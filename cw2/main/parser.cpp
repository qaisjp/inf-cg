#include "parser.h"
#include <stdexcept>

#include "cameras/Pinhole.h"
#include "core/Camera.h"
#include "core/LightSource.h"
#include "core/Scene.h"
#include "lights/AreaLight.h"
#include "lights/PointLight.h"
#include "shapes/shapes.h"

#define GETSTR_TO_STD(X) std::string(X.GetString(), X.GetStringLength())

namespace rt {

Parser::Parser() {}
Parser::~Parser() {}

template <class T>
std::vector<T*>* Parser::ParseMany(rapidjson::Value& value) {
    auto xs = new std::vector<T*>;
    for (auto& val : value.GetArray()) {
        T* x;
        Parse(x, val);
        if (x) {
            xs->push_back(x);
        }
    }
    return xs;
}

void Parser::Parse(Scene*& scene, rapidjson::Value& value) {
    scene = new Scene();

    for (auto& obj : value.GetObject()) {
        const auto& key = obj.name;

        std::printf("Parsing scene.%s\n", GETSTR_TO_STD(key).data());
        if (key == "shapes") {
            ParseMany<Shape>(obj.value);
        } else if (key == "materials") {
            ParseMany<Material>(obj.value);
        } else if (key == "lightsources") {
            ParseMany<LightSource>(obj.value);
        }
    }
}

void Parser::Parse(Camera*& cam, rapidjson::Value& value) {
    const auto& d = value.GetObject();

    if (!d.HasMember("type")) {
        throw std::invalid_argument("shape missing type");
    }

    const auto& type = d["type"];
    if (type == "pinhole") {
        Pinhole* pinhole = nullptr;
        Parse(pinhole, value);
        cam = pinhole;
    } else {
        throw std::invalid_argument("unknown type: " + GETSTR_TO_STD(type));
    }
}

void Parser::Parse(Pinhole*& cam, rapidjson::Value& d) {
    auto width = d["width"].GetInt();
    auto height = d["height"].GetInt();
    auto fov = d["fov"].GetInt();
    cam = new Pinhole(width, height, fov);
}

void Parser::Parse(Shape*& shape, rapidjson::Value& value) {
    const auto& d = value.GetObject();

    if (!d.HasMember("type")) {
        throw std::invalid_argument("shape missing type");
    }

    const auto& type = d["type"];
    if (type == "sphere") {
        Sphere* sphere = nullptr;
        Parse(sphere, value);
        shape = sphere;
    } else {
        throw std::invalid_argument("unknown type: " + GETSTR_TO_STD(type));
    }
}

void Parser::Parse(Sphere*& sphere, rapidjson::Value& d) {
    auto center = ParseVec3f(d["center"]);
    auto radius = d["radius"].GetFloat();
    sphere = new Sphere(center, radius);
}

void Parser::Parse(LightSource*& light, rapidjson::Value& value) {
    const auto& d = value.GetObject();

    if (!d.HasMember("type")) {
        throw std::invalid_argument("LightSource missing type");
    }

    const auto& type = d["type"];
    if (type == "pointlight") {
        PointLight* point = nullptr;
        Parse(point, value);
        light = point;
    } else {
        throw std::invalid_argument("unknown type: " + GETSTR_TO_STD(type));
    }
}

void Parser::Parse(PointLight*& light, rapidjson::Value& d) {
    auto pos = ParseVec3f(d["position"]);
    auto intensity = ParseVec3f(d["intensity"]);
    light = new PointLight();  // pos, intensity);
}

void Parser::Parse(Material*& mat, rapidjson::Value& d) {
    // TODO
    mat = nullptr;
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