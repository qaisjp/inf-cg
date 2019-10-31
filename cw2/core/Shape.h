#pragma once

#include "core/Material.h"
#include "core/RayHitStructs.h"

namespace rt {

class Shape {
   public:
    //
    // Constructors
    //
    Shape() {};

    //
    // Destructor (must be overriden in subclass)
    //
    virtual ~Shape() {}

    //
    // Shape abstract methods (to be implemented by subclasses)
    //
    virtual Hit intersect(Ray) = 0;

   protected:
    Material* material;
};

}  // namespace rt
