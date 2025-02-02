/*
 * Sphere.cpp
 *
 *
 */
#include "Sphere.h"

namespace rt {

Sphere::Sphere(Vec3f center, float radius) : center(center), radius(radius) {
    std::printf("Sphere(center=Vecf(%f, %f, %f) radius=%f)\n", center.x,
                center.y, center.z, radius);
}

Sphere::~Sphere() {}

/**
 * Computes whether a ray hit the specific instance of a sphere shape and
 *returns the hit data
 *
 * @param ray cast ray to check for intersection with shape
 *
 * @return hit struct containing intersection information
 *
 */
Hit Sphere::intersect(Ray ray) {
    Hit h;
    //-----------to be implemented -------------

    return h;
}

}  // namespace rt
