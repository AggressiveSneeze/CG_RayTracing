//
// Created by jadams on 1/11/16.
//

#include "scene.h"

//TODO: see if no constant allready defined for this perpuse
#define T_MAX 1


Scene::Scene() { };
Scene::Scene(Color3d & color, AmbientLight & light, double cutoffAngle):_background(color),
                                                                        _ambientLight(light),_cutoffAngle(cutoffAngle){};


typedef struct IntersectProps{
    //Object** object, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor
    Object** object;
    double t;
    Point3d P;
    Vector3d N;
    Color3d texColor;

    IntersectProps() : object(nullptr) {}
}IntersectProps;

void Scene::add_object(Object * obj) {
    _objects.push_back(obj);
};

void Scene::add_light(PointLight *light) {
    _lights.push_back(light);
}

Color3d Scene::trace_ray(Ray ray, double vis /*= 1.0*/) const {
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
    //std::cout<<"potato";
    if(vis < MINIMAL_VIS)
    {
        return _background;
    }

    vis *= RECURSION_FACTOR;
    IntersectProps nearest_obj_props;
    bool isIntersect = findNearestObject(ray, nearest_obj_props.object, nearest_obj_props.t,
                                        nearest_obj_props.P, nearest_obj_props.N, nearest_obj_props.texColor);
    Color3d reflection_color = _background;
    Color3d refraction_color;
    Color3d enviorment_color;
    if(isIntersect)
    {
        std::cout<<"we have an itnersection. "<<std::endl;
        reflection_color = calcReflection(ray, nearest_obj_props.P, nearest_obj_props.N, **nearest_obj_props.object, vis);
        refraction_color = calcRefraction(ray, nearest_obj_props.P, nearest_obj_props.N, **nearest_obj_props.object, vis);
    }
    else
    {
        return _background;
    }
    //iterate over lights to find if any object shadows the object, if not calculate phong
    /*
     * ambient term: obj.AmbientColor * scene.ambientLight +
        //per each visible light source:
        diffuse term: obj.diffuseColor * lightColor * <L , N> +
        specular term: obj.specularColor * lightColor *<R,L>^obj.shininess
     */
    Color3d ambient = (*nearest_obj_props.object)->getAmbient() * _ambientLight._color;
    Color3d diffuse = COLOR_BLACK;
    Color3d specular = COLOR_BLACK;
    for(vector<PointLight *>::const_iterator it = _lights.cbegin(); it != _lights.cend(); it++)
    {
        //Ray in the direcation of the light from the point P
        Vector3d lightDir = (*it)->_position - nearest_obj_props.P;
        Ray ray = Ray(nearest_obj_props.P, lightDir);
        Vector3d Rl = (ray(1) - (((nearest_obj_props.N|ray(1)) * nearest_obj_props.N))*2).normalize();
        bool intersect = findNearestObject(ray, nearest_obj_props.object, nearest_obj_props.t,
                                            nearest_obj_props.P, nearest_obj_props.N, nearest_obj_props.texColor);
        if(intersect)
        {
            continue;
        }
        else
        {
          diffuse += (*object)->getDiffuse() * (*it)->_color  * (lightDir.normalize()|N.normalize());
            specular += (*object)->getSpecular() * (*it)->_color * pow((Rl | lightDir.normalize()), (*object)->shining());
        }
    }
    //TODO: calculate shadow for each light source and phong
    return (ambient + diffuse + specular) + reflection_color + refraction_color;
}

bool Scene::findNearestObject(IN Ray ray, OUT Object** object, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor) const
{
    double dist = INF;
    double nearestT;
    Point3d nearestP;
    Vector3d nearestN;
    Color3d nearestTexColor;

    //iterate over all the objects of the scene and determine which is the nearest
    for(vector<Object *>::const_iterator it = _objects.begin(); it != _objects.end(); it++)
    {
        Object * currObj= *it;
        int intersect = currObj->intersect(ray, T_MAX, nearestT, nearestP, nearestN, nearestTexColor);
        if(intersect)
        {
            double tempDist = (ray.O() - nearestP).length();
            //If the object is the nearest update all the OUTs
            if(tempDist < dist)
            {
                dist = tempDist;
                object = &currObj;
                t = nearestT;
                P = nearestP;
                N = nearestN;
                texColor = nearestTexColor;
            }
        }
    }

    if(dist != INF)
    {
        return true;
    }
    return false;
}

Color3d Scene::calcReflection(const Ray& ray, const Point3d& P, const Vector3d& N, const Object& object, double vis /*= 1.0*/, bool isCritical /*= false*/) const {
    //psuedocode
//    c1 = -dot_product( N, V )
//    Rl = V + (2 * N * c1 )
    //Rl=-Rl;
    //new_ray=Ray(P,normalise(Rl))
    //return trace_ray(ray,..)
    Point3d rayDir = ray(1.0);
    Vector3d Rl;
    if(isCritical)
    {
        double cos_theta_i = (N|rayDir) * object.getIndex();
        Rl = (rayDir - (cos_theta_i * N)*2).normalize();
    }
    else
    {
        Rl = (rayDir - ((N|rayDir) * N)*2).normalize();
    }
    Ray newRay = Ray(P, Rl);
    if(_numberOfRefRays == 1)
    {
        return object.getReflection() * trace_ray(newRay, vis);
    }
    //ToDo: cone random rays

}

Color3d Scene::calcRefraction(const Ray& ray, const Point3d& P, const Vector3d& N, const Object& object, double vis /*= 1.0*/) const {
    //snell's law
    //ToDO: is _index is n_1/n_2?
    //Todo: where should we take 1/index?
    double index = object.getIndex();
    Point3d rayDir = ray(1);
    double cos_theta_i = (N|rayDir) * index;
    double cos_theta_t = 1 - pow(index,2) * (1 - (N | rayDir) * (N | rayDir));
    if(cos_theta_t < 0)
    {
        return calcReflection(ray, P, N, object, vis, true);
    }
    Vector3d Re = (cos_theta_i - sqrt(cos_theta_t)) * N - rayDir * index;
    Ray newRay = Ray(P, Re);
    if(_numberOfRefRays == 1)
    {
        return object.getTransparency() * trace_ray(newRay, vis);
    }
}






