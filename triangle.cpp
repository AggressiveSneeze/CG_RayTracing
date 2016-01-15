//
// Created by jadams on 1/13/16.
//
#include "triangle.h"
Triangle::Triangle(Point3d p0, Point3d p1, Point3d p2): _p0(p0), _p1(p1),_p2(p2) {};



int Triangle::intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N) {

    //ported from http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf

    //find vectors for two edges sharing vert0
    Vector3d edge1=_p1-_p0, edge2=_p2-_p1;
    //begin calculating determined - also used to calculate U parameter.
    Vector3d pvec=ray.D()%edge2; //cross product

    //if determinant is near zero, ray lies in plane of triangle.
    float det=edge1|pvec;
    //not sure how to determine which side of triangle is inside/outside mesh, so using non culling branch for the time
    //being.
    if (det > -EPS && det<EPS) return 0;

    float inv_det=1.0/det;

    //calculate distance from vert0 to ray origin.
    Vector3d tvec=ray.O()-_p0;
    //calculate U parameter and test bounds
    float u=(tvec|pvec)*inv_det;
    if (u<0.0 || u>1.0) return 0;

    //prepare to test v parameter
    Vector3d qvec=tvec%edge1;
    //calculate v parameter and test bounds
    float v=(ray.D()|qvec)*inv_det;
    if(v<0.0 || v>1.0) return 0;
    //calculate t since ray intersects triangle
    t=(edge2|qvec)*inv_det;
    //confirm that t isn't too big (i.e triangle isn't too far away)
    if (t>tMax) return 0;

    //port ends here, james continuing:
    //point of intersection is given by substitution of u,v into the formula for barycentric.
    P=_p0*(1-u-v)+_p1*u+_p2*v;
    //TODO:: add the normal
    return 1;
};