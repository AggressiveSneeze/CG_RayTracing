//
// Created by shirelga on 1/11/16.
//


#include "camera.h"
//constructors
Camera::Camera(Point3d & pos,Point3d & coi, Vector3d & up, double fov, size_t samples_per_pixel = 1):
        _position(pos),_coi(coi),_fov_h(fov),_samples_per_pixel(samples_per_pixel)
{};

//Camera::Camera(): _position(){};

//setter
void Camera::setSamplesPerPixel(size_t samples_per_pixel) {
    _samples_per_pixel=samples_per_pixel;
};

// render - create a bitmap image with the given properties from the given scene
void Camera::render(size_t row_start, size_t number_of_rows, BImage& img, Scene & scene) {
    //psuedocode
    //for each pixel
    //  for each ray from viewport
    //      img[pixel]=scene.race_tray

};
