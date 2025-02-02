/*
 * Camera.h
 *
 */

#pragma once

#include <math/geometry.h>
#include "rapidjson/document.h"

using namespace rapidjson;

namespace rt {

    class Camera {
    public:
        //
        // Constructors
        //
        Camera() {};

        Camera(int width, int height, int fov)
                : height(height), width(width), fov(fov) {};

        //
        // Destructor
        //
        virtual ~Camera();

        //
        // print function (to be implemented by the subclasses )
        //
        virtual void printCamera() = 0;

        //
        // other camera functions (to complete)
        //

        //
        // Getters and Setters
        //
        int getHeight() const { return height; }

        void setHeight(int height) { this->height = height; }

        int getWidth() const { return width; }

        void setWidth(int width) { this->width = width; }

        static Matrix44<float> lookAt(const Vec3<float> &from, const Vec3<float> &to, const Vec3<float> &tmp);

    protected:
        //
        // camera members
        //
        int height;
        int width;
        
    public:
        int getFov() const {
            return fov;
        }

    protected:
        int fov;  // field of view
    };

}  // namespace rt

