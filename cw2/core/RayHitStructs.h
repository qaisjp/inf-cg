#pragma once

#include "math/geometry.h"

namespace rt {

/*
 * Ray structure definition
 */
enum RayType { PRIMARY, SECONDARY, SHADOW };

struct Ray {
    RayType raytype;

    //----------Ray variables to be filled------
};

struct Hit {
    Vec3f point;  // point where ray hits a shape
                  //----------Hit variables to be filled------
};
}  // namespace rt
