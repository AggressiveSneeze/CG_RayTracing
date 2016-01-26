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
    view_port_size=(_coi - _position).norm() * tan(_fov_h) * 2;
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
    // Calculate image and camera properties
//    std::cout<<"height"<<img.height()<<" width"<<img.width()<<std::endl;
    int width = img.getWidth(), height = img.getHeight();

    double fov_w = (height / width) * _fov_h;

    Vector3d lens_dir = _coi - _position;
    Vector3d lens_dir_n = lens_dir.normalize();
    double pix_size_x = (lens_dir.length() * tan(fov_w) * 2)/width;
    double pix_size_y = (lens_dir.length() * tan(_fov_h) * 2)/height;

    //rotation matrix according to up
    Vector3d R3 = lens_dir_n;
    Vector3d R2 = (_up - (_up.normalized() | lens_dir_n) * lens_dir_n).normalize();
    Vector3d R1 =  (R2 % lens_dir_n).normalize();



    Ray r;
    Vector3d dir;
    Color3d pix_color;
    Bpixel temp_pixel;
    Color3d average_color=Color3d(0.0,0.0,0.0);
    for(size_t row = row_start; row < row_start + number_of_rows; row++)
        for(size_t col = 0; col < img.width(); col++) {
//            std::cout << "Pixel: " << row << "," << col << " " << "ray: " << (col + 0.5 - img.width() / 2) * pix_size_x << " ," << (row + 0.5 - img.height() / 2) * pix_size_y << std::endl;
            if (_samples_per_pixel == 1) {
//
                double middle_pixel_x = (col + 0.5 - width / 2) * pix_size_x;
                double middle_pixel_y = (row + 0.5 - height / 2) * pix_size_y;

                dir = (R3 - (R2 * middle_pixel_y + R1 * middle_pixel_x));
//                float z=dir[Z], y=dir[Y];
//                dir[Y]=y*cos(3*PI/2)-z*sin(3*PI/2);
//                dir[Z]=y*sin(3*PI/2)+z*cos(3*PI/2);
                r = Ray(_position, dir);
                pix_color = scene.trace_ray(r) * 255;
                temp_pixel = Bpixel((uchar) (pix_color[0]), (uchar) (pix_color[1]), (uchar) (pix_color[2]));
                img(static_cast<int>(col), static_cast<int>(row)) = temp_pixel;
            }
            else {
                for (int j = 0; j < _samples_per_pixel; j++) {
                    //number of rays is more than one.ß
                    Vector3d temp =randInPixelPoint(pix_size_x, pix_size_y, row, col);
//                    std::cout<<temp<<std::endl;
                    double point_in__pixel_x = (col + temp[X] - width / 2) * pix_size_x;
                    double point_in_pixel_y = (row + temp[Y] - height / 2) * pix_size_y;
                    dir = (R3 - (R2 * point_in_pixel_y + R1 * point_in__pixel_x));
                    r = Ray(_position, dir);
                    average_color+=scene.trace_ray(r) * 255;
                }
                average_color*=(1.0/_samples_per_pixel);
                temp_pixel = Bpixel((uchar) (average_color[0]), (uchar) (average_color[1]), (uchar) (average_color[2]));
                img(static_cast<int>(col), static_cast<int>(row))=temp_pixel;
            }
        }
//    std::cout<<"scene size: "<<view_port_size<<std::endl;

};

static Point3d randInPixelPoint(double width, double height, size_t row, size_t col)
{
    double rand_x = (fmod((double)rand(),width))/width;
    double rand_y = (fmod((double)rand(),height))/height;
    return Point3d(rand_x, rand_y, 1.f);
}


