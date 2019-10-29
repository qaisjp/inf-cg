/*
 * RayTracer.h
 *
 */
#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include "core/Camera.h"
#include "core/Scene.h"
#include "math/geometry.h"

namespace rt {

/*
 * Raytracer class declaration
 */
class RayTracer {
   public:
    RayTracer();

    static Vec3f* render(Camera* camera, Scene* scene, int nbounces);
    static Vec3f* tonemap(Vec3f* pixelbuffer);

   private:
};

}  // namespace rt

#endif /* RAYTRACER_H_ */
