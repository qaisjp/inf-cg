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
    Vec3f *RayTracer::render(Camera *camera, Scene *scene, int nbounces) {
        auto width = camera->getWidth();
        auto height = camera->getHeight();

        auto *pixelbuffer = new Vec3f[width * height];

        //----------main rendering function to be filled------
        Matrix44f cameraToWorld;
        Vec3f *framebuffer = new Vec3f[width * height];
        Vec3f *pix = framebuffer;
        float scale = tan((camera->getFov() * 0.5) * M_PI / 180.0f);
        float imageAspectRatio = width / (float) height;
        Vec3f orig;
        cameraToWorld.multVecMatrix(Vec3f(0), orig);

        for (uint32_t j = 0; j < height; ++j) {
            for (uint32_t i = 0; i < width; ++i) {
                float x = (2 * (i + 0.5f) / (float) width - 1) * imageAspectRatio * scale;
                float y = (1 - 2 * (j + 0.5f) / (float) height) * scale;
                Vec3f dir;
                cameraToWorld.multDirMatrix(Vec3f(x, y, -1), dir);
                dir.normalize();
                *(pix++) = castRay(orig, dir, objects, lights, options, 0);
            }
        }

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
    Vec3f *RayTracer::tonemap(size_t size, Vec3f *pixelbuffer) {
        //---------tonemapping function to be filled--------

        for (size_t i = 0; i < size; i++) {
            Vec3f *v = &pixelbuffer[i];
            *v = *v * 255;
        }

        return pixelbuffer;
    }

}  // namespace rt
