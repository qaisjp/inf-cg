/*
 * RayTracer.cpp
 *
 */
#include "RayTracer.h"

namespace rt {

/**
 * Performs ray tracing to render a photorealistic scene
 *
 * @param camera the camera viewing the scene
 * @param scene the scene to render, including objects and lightsources
 * @param nbounces the number of bounces to consider for raytracing
 *
 * @return a pixel buffer containing pixel values in linear RGB format
 */
Vec3f* RayTracer::render(Camera* camera, Scene* scene, int nbounces) {
    Vec3f* pixelbuffer = new Vec3f[camera->getWidth() * camera->getHeight()];

    //----------main rendering function to be filled------


    return pixelbuffer;
}

/**
 * Tonemaps the rendered image (conversion of linear RGB values [0-1] to low
 *dynamic range [0-255]
 *
 * @param pixelbuffer the array of size width*height containing linear RGB pixel
 *values
 *
 * @return the tonemapped image
 */
Vec3f* RayTracer::tonemap(size_t size, Vec3f* pixelbuffer) {
    //---------tonemapping function to be filled--------

    for (size_t i = 0; i < size; i++)
    {
        Vec3f* v = &pixelbuffer[i];
        // v->x = 1;
        // v->y = 0;
        // v->z = 0;
        *v = *v * 255;
    }

    return pixelbuffer;
}

}  // namespace rt
