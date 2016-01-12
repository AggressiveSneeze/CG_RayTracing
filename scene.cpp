//
// Created by jadams on 1/11/16.
//

#include "scene.h"


void Scene::add_object(Object * obj) {
    _objects.push_back(obj);
};

void Scene::add_light(PointLight *light) {
    _lights.push_back(light);
}

Color3d Scene::trace_ray(Ray ray, double vis = 1.0) const {
    //psuedocode
    //findNearestObject()
    //if false return some colour (maybe background).
//  else we have the object
//    if ( object is reflective )
//    reflect_color = trace_ray( get_reflected_ray( original_ray, obj ) )
//    if ( object is refractive )
//    refract_color = trace_ray( get_refracted_ray( original_ray, obj ) )
//
//    return ( combine_colors( point_color, reflect_color, refract_color ))

}

Color3d calcReflection(const Ray& ray, const Point3d& P, const Vector3d& N, const Object& object, double vis = 1.0, bool isCritical = false) const {
    //TODO: Check the minus on the reflected Rl.
    //psuedocode
//    c1 = -dot_product( N, V )
//    Rl = V + (2 * N * c1 )
    //Rl=-Rl;
    //new_ray=Ray(P,normalise(Rl))
    //return trace_ray(ray,..)
    //maybe the second argument is a strength/recursion level indicator.
    //TODO also need to take into account ray splitting/creation of new rays and phong model

}

Color3d calcRefraction(const
 Ray& ray, const Point3d& P, const Vector3d& N, const Object& object, double vis = 1.0) const {
    //snell's law

    
}






