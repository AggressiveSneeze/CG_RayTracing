#include "sphere.h"

Sphere::Sphere(): _C(Vector3d(0.0, 0.0, 0.0)), _r(1){};

Sphere::Sphere(Point3d C, double r): _C(C), _r(r){};

int Sphere::intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor) {
    //TODO check openmesh usage is okay
    float a=ray.D().x*ray.D().x+ray.D().y*ray.D().y+ray.D().z*ray.D().z;
    float b=2*(ray.O()-_C)|ray.D();
    float c=(ray.O()-_C)|(ray.O()-_C)-_r*_r;
    float d=b*b-4*c;

    if (d<0.0) return 0;
    float rootD=sqrt(d);
    float t0=0.5*(-b-rootD);
    float t1=0.5*(-b+rootD);

    if(t0>=0 && t0<=tMax) {
        t=t0;
        P=ray.O()+t*ray.D();
        N=P-_C;
        //TODO texcolor
        return 1;
    }
    if(t1>=0 && t1<=tMax) {
        t=t1;
        P=ray.O()+t*ray.D();
        N=P-_C;
        //TODO texcolor
        return 1;
    }
    //TODO do we need to do default assignments for the outs?
    return 0;
};


Color3d textureDiffuse(const Point3d& P) {

};

