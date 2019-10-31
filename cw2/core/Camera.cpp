/*
 * Camera.cpp
 *
 */
#include <iostream>

#include "Camera.h"
#include "cameras/Pinhole.h"
#include "cameras/ThinLens.h"
#include "geometry.h"

namespace rt {

Camera::~Camera(){};

// From
// https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/lookat-function
// Matrix44f Camera::lookAt(const Vec3f& from, const Vec3f& to,
//                          const Vec3f& tmp = Vec3f(0, 1, 0)) {
//     Vec3f forward = normalize(from - to);
//     Vec3f right = crossProduct(normalize(tmp), forward);
//     Vec3f up = crossProduct(forward, right);

//     Matrix44f camToWorld;

//     camToWorld[0][0] = right.x;
//     camToWorld[0][1] = right.y;
//     camToWorld[0][2] = right.z;
//     camToWorld[1][0] = up.x;
//     camToWorld[1][1] = up.y;
//     camToWorld[1][2] = up.z;
//     camToWorld[2][0] = forward.x;
//     camToWorld[2][1] = forward.y;
//     camToWorld[2][2] = forward.z;

//     camToWorld[3][0] = from.x;
//     camToWorld[3][1] = from.y;
//     camToWorld[3][2] = from.z;

//     return camToWorld;
// }

}  // namespace rt
