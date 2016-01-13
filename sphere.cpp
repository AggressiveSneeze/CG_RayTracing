#include <OpenMesh/Core/Geometry/VectorT.hh>
#include "sphere.h"

Sphere::Sphere(): _C(Vector3d(0.0, 0.0, 0.0)), _r(1){};

Sphere::Sphere(Point3d C, double r): _C(C), _r(r){};

int Sphere::intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor) {
    //TODO check openmesh usage is okay
//    Vector3d normalizedRayD = ray.D().normalized();
    double a = (ray.D() | ray.D());
    double b = 2*(ray.O()-_C)|ray.D();
    double rCube = pow(_r, 2);
    double c = (ray.O()-_C)|((ray.O()-_C)- Vector3d(rCube, rCube, rCube));
    double d = b*b-4*c;

    if (d<0.0) return 0;
    double rootD=sqrt(d);
    double t0=0.5*(-b-rootD);
    double t1=0.5*(-b+rootD);

    if(t0>=0 && t0<=tMax) {
        t=t0;
        P= ray(t);
        N=P-_C;
        return INTERSECTION;
    }
    if(t1>=0 && t1<=tMax) {
        t=t1;
        P= ray(t);
        N=P-_C;
        //TODO texcolor
        return INTERSECTION;
    }
    return NO_INTERSECTION;
};


Color3d textureDiffuse(const Point3d& P) {

};

