//
// Created by shirelga on 1/11/16.
//


#include "camera.h"

//TODO:check why declared with forward dec
#include "bimage.h"
#include "scene.h"
#include "time.h"

static Point3d randInPixelPoint(double width, double height, size_t row, size_t col);

//constructors
Camera::Camera(Point3d & pos,Point3d & coi, Vector3d & up, double fov, size_t samples_per_pixel /*= 1*/):
        _position(pos),_coi(coi),_up(up),_fov_h(fov),_samples_per_pixel(samples_per_pixel)
{
    //calculate viewpoint-size
    view_port_size=(_coi-_position).length()*tan(_fov_h)*2;


};

Camera::Camera() {};

//Camera::Camera(): _position(){};

//setter
void Camera::setSamplesPerPixel(size_t samples_per_pixel) {
    _samples_per_pixel=samples_per_pixel;
};


// render - create a bitmap image with the given properties from the given scene
void Camera::render(size_t row_start, size_t number_of_rows, BImage& img, Scene & scene) const{

    std::srand(time(NULL));
    double pix_size_x = view_port_size/img.width();
    double pix_size_y = view_port_size/img.height();
    Ray r;
    Point3d origin;
    Color3d pix_color;
    Bpixel temp_pixel;
    Color3d average_color=Color3d(0.0,0.0,0.0);
    for(size_t i = 0; i < img.width(); i++) {
        if (_samples_per_pixel == 1) {
            origin = Point3d(((i + 0.5) * pix_size_x), ((row_start + 0.5) * pix_size_y), 0.f);
            r = Ray(origin, origin - _position);
            pix_color = scene.trace_ray(r);
            temp_pixel = Bpixel((uchar) (pix_color[0]), (uchar) (pix_color[1]), (uchar) (pix_color[2]));
            img(row_start, i) = temp_pixel;
        }
        else {
            for (int j = 0; j < _samples_per_pixel; j++) {
                //number of rays is more than one.
                origin=randInPixelPoint(pix_size_x, pix_size_y, row_start, i);
                r = Ray(origin,origin - _position);
                average_color+=scene.trace_ray(r);
            }
            average_color*=(1.0/_samples_per_pixel);
            temp_pixel = Bpixel((uchar) (average_color[0]), (uchar) (average_color[1]), (uchar) (average_color[2]));
            img(row_start,i)=temp_pixel;
        }
    }

};

static Point3d randInPixelPoint(double width, double height, size_t row, size_t col)
{
    double rand_x = row+(fmod((double)rand(),width));
    double rand_y = col+(fmod((double)rand(),height));
    return Point3d(rand_x, rand_y, 0.f);
}
