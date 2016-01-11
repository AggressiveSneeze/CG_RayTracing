#include "sphere.h"

Sphere::Sphere(): _C(Vector3d(0.0, 0.0, 0.0)), _r(1){};

Sphere::Sphere(Point3d C, double r): _C(C), _r(r){};

