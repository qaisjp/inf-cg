#pragma once

#include "core/RayHitStructs.h"
#include "core/Shape.h"
#include "math/geometry.h"

namespace rt {

class Sphere : public Shape {
   public:
    //
    // Constructors
    //
    Sphere(Vec3f center, float radius);

    ~Sphere();

    //
    // Functions that need to be implemented, since Sphere is a subclass of
    // Shape
    //
    Hit intersect(Ray ray) override;

   private:
    Vec3f center;
    float radius;
};

}  // namespace rt
