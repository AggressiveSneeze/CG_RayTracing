//
// Created by shirelga on 1/11/16.
//


#include "camera.h"

//TODO:check why declared with forward dec
#include "bimage.h"
#include "scene.h"
#include"time.h"

static Point3d randInPixelPoint(double width, double height, size_t row, size_t col);

//constructors
Camera::Camera(Point3d & pos,Point3d & coi, Vector3d & up, double fov, size_t samples_per_pixel /*= 1*/):
        _position(pos),_coi(coi),_up(up),_fov_h(fov),_samples_per_pixel(samples_per_pixel)
{};

//Camera::Camera(): _position(){};

//setter
void Camera::setSamplesPerPixel(size_t samples_per_pixel) {
    _samples_per_pixel=samples_per_pixel;
};

// render - create a bitmap image with the given properties from the given scene
void Camera::render(size_t row_start, size_t number_of_rows, BImage& img, Scene & scene) const{
    std::srand(time(NULL));
    double pix_size_x = (2 * tan(_fov_h))/img.width();
    double pix_size_y = (2 * tan(_fov_h))/img.height();
    //Bpixel temp_pixel=new Bpixel();
    //psuedocode
    //for each pixel
    //  for each ray from viewport
    //      img[pixel]=scene.race_tray
    for(size_t i = 0; i < img.width(); i++)
    {
        for(int j = 0; j < _samples_per_pixel; j++)
        {
            Ray r;
            if(_samples_per_pixel == 1)
            {
                r = Ray(Point3d((i * pix_size_x)/2.f, (row_start * pix_size_y)/2.f, 0.f), _coi - _position);
            }
            else
            {
                r = Ray(randInPixelPoint(pix_size_x, pix_size_y, row_start, i), _coi - _position);
            }
            Color3d pix_color = scene.trace_ray(r);
            //TODO This isn't quite right.
            //temp_pixel=new Bpixel((uchar)(pix_color[0]), (uchar)(pix_color[1]), (uchar)(pix_color[2]));
//            img(row_start, j) = uchar(pix_color[0], pix_color[1], pix_color[2]);
            //img(row_start,j)=(new Bpixel((uchar)(pix_color[0]), (uchar)(pix_color[1]), (uchar)(pix_color[2])));
            //temp so i can keep mock-compiling.
                img(row_start,j)=5;
        }
    }

};

static Point3d randInPixelPoint(double width, double height, size_t row, size_t col)
{
    double rand_x = col * ((double)rand() / width);
    double rand_y = col * ((double)rand() / height);

    rand_x = (col * width) + rand_x * (width);
    rand_y = (col * height) + rand_y * (height);

    return Point3d(rand_x, rand_y, 0.f);
}
