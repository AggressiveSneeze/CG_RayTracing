//#include <OpenMesh/Core/Geometry/VectorT.hh>
#include "sphere.h"

Sphere::Sphere() {

//    _C=Vector3d(0.0, 0.0, 0.0);
//    _r=1.0;
};
Sphere::Sphere(Point3d C, double r): _C(C), _r(r){};

//decon
Sphere::~Sphere(){};

int Sphere::intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor) {
    //TODO check openmesh usage is okay
//    Vector3d normalizedRayD = ray.D().normalized();
    double a = (ray.D() | ray.D());
    double b = (ray.O()-_C|ray.D())* 2.0;
    double rCube = pow(_r, 2.0);
    double c = ((ray.O()-_C)|((ray.O()-_C)))- rCube;
    double d = b*b-4.0*c;

    if (d<EPS) return NO_INTERSECTION;
    double rootD=sqrt(d);
    double t0=0.5*(-b-rootD);
    double t1=0.5*(-b+rootD);
//    std::cout<<"t0 is: "<<t0<<",";
//    std::cout<<"t1 is: "<<t1<<","<<"tmax is : "<<tMax<<std::endl;

    if(t0>=EPS && t0<=tMax) {
        t=t0;
        P= ray(t);
        N=(P - _C)/_r;
        return INTERSECTION;
    }
    if(t1>=EPS && t1<=tMax) {
        t=t1;
        P= ray(t);
        N=(P - _C)/_r;
        //TODO texcolor
        return INTERSECTION;
    }
    return NO_INTERSECTION;
};


Color3d textureDiffuse(const Point3d& P) {
};

