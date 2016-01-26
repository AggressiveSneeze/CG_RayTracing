//
// Created by jadams on 1/11/16.
//

#include "scene.h"

//TODO: see if no constant allready defined for this perpuse

double  t_max = INF;


Scene::Scene() { };
Scene::Scene(Color3d & color, AmbientLight & light, double cutoffAngle):_background(color),
                                                                        _ambientLight(light),
                                                                        _cutoffAngle(cutoffAngle)
                                                                        {};


//typedef struct IntersectProps{
//    //Object** object, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor
//    Object** object;
//    double t;
//    Point3d P;
//    Vector3d N;
//    Color3d texColor;
//
//    IntersectProps() : object(nullptr), t(0) {}
//}IntersectProps;

void Scene::add_object(Object * obj) {
    _objects.push_back(obj);
};

void Scene::add_light(PointLight *light) {
    _lights.push_back(light);
}

void Scene::add_sphere_light(SphereLight * light)
{
    _sphereLights.push_back(light);
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
//    if(vis == 1)
//    {
//        t_max = INF;
//    }
//    double tmp_t_max;
    if(vis < MINIMAL_VIS)
    {
        return _background;
    }

    vis *= RECURSION_FACTOR;
//    IntersectProps nearest_obj_props;
    Object *object = nullptr;
    double t;
    Point3d P;
    Vector3d N;
    Color3d texColor;
    bool isIntersect = findNearestObject(ray, &object, t,
                                        P, N, texColor);

    Color3d reflection_color = _background;
    Color3d refraction_color = _background;
    Color3d shading_color = COLOR_BLACK;

    if(isIntersect)
    {

        shading_color = phongShading(P, N, *object);

        reflection_color = calcReflection(ray, P, N, *object, vis * RECURSION_FACTOR);

        refraction_color = calcRefraction(ray, P, N, *object, vis * RECURSION_FACTOR);

    }
    else
    {
        return _background;
    }
//    std::cout<<"color comp ref: "<<reflection_color<<",refraction: "<<refraction_color<<", diff"<<diffuse<<", spec"<<specular<<", amb: "<<ambient<<std::endl;
//    //TODO: calculate shadow for each light source and phong
////    std::cout<<(ambient + diffuse + specular) + reflection_color + refraction_color<<std::endl;
//    return (ambient + diffuse + specular) + reflection_color + refraction_color;
//    Color3d color = shading_color + reflection_color + refraction_color;
    Color3d color = shading_color + reflection_color + refraction_color;

    for(int i = 0; i < 3; i++)
    {
        if(color[i] < 0.0)
        {
            color[i] = 0.0;
        }
        else if(color[i] > 1.0)
        {
            color[i] = 1.0;
        }
    }
//    std::cout<<color;
    return color * vis;
//    return (reflection_color) * 255;
}

bool Scene:: findNearestObject(IN Ray ray, OUT Object** object, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor) const
{
    double t_max = INF;
    bool isIntersect = false;
    double nearestT;
    Point3d nearestP;
    Vector3d nearestN;
    Color3d nearestTexColor;
    //iterate over all the objects of the scene and determine which is the nearest
    for(vector<Object *>::const_iterator it = _objects.begin(); it != _objects.end(); it++)
    {
        int intersect = (*it)->intersect(ray, t_max, nearestT, nearestP, nearestN, nearestTexColor);
        if(intersect)
        {
            isIntersect = true;
            t_max = t;
            *object = *it;
            t = nearestT;
            P = nearestP;
            N = nearestN;
            texColor = nearestTexColor;
        }
    }
    if(isIntersect) return true;

    return false;
}

Color3d Scene::phongShading(const Point3d& P, const Vector3d& N, Object& object) const
{
    Color3d ambient = object.ambient() * _ambientLight._color;
    Color3d diffuse = COLOR_BLACK;
    Color3d specular = COLOR_BLACK;
    for(auto light : _lights)
    {
        //Ray in the direcation of the light from the point P
        Vector3d L = (light->_position - P).normalize();
        Ray ray = Ray(P, L);
//            Vector3d raydir = ray(1).normalize();
        Vector3d Rl = ((((N.normalized()|L) * N))*2 - L).normalize();
        //dummy vars
        double t_0;
        Object * obj;
        Point3d P_0;
        Vector3d N_0;
        Color3d texColor;

        int intersect = NO_INTERSECTION;
        for(auto o : _objects)
        {
            intersect = o->intersect(ray, INF, t_0,
                                           P_0, N_0, texColor);
        }

        if(intersect)
        {
            continue;
        }
        else
        {
            diffuse += object.getDiffuse() * light->_color  * std::max(0.0, (L|N.normalized()));
            specular += object.getSpecular() * light->_color * pow(std::max(0.0, (Rl | L)), object.shining());
        }
    }

    for(auto light : _sphereLights)
    {
        for(int i = 0; i < 20; i++)
        {
            Point3d sphereRay = sphereLightRandomRay(*light);
            Vector3d L = (sphereRay - P).normalize();
            Ray ray = Ray(P, L);
            Vector3d Rl = ((((N.normalized()|L) * N))*2 - L).normalize();
            //dummy vars
            double t_0;
            Object * obj;
            Point3d P_0;
            Vector3d N_0;
            Color3d texColor;

            int intersect = NO_INTERSECTION;
            for(auto o : _objects)
            {
                intersect = o->intersect(ray, INF, t_0,
                                         P_0, N_0, texColor);
            }

            if(intersect)
            {
                continue;
            }
            else
            {
                diffuse += object.getDiffuse() * light->_color  * std::max(0.0, (L|N.normalized()));
                specular += object.getSpecular() * light->_color * pow(std::max(0.0, (Rl | L)), object.shining());
            }

        }
    }
    return ambient + diffuse + specular;
}



Color3d Scene::calcReflection(const Ray& ray, const Point3d& P, const Vector3d& N, const Object& object, double vis /*= 1.0*/, bool isCritical /*= false*/) const {
    //psuedocode
//    c1 = -dot_product( N, V )
//    Rl = V + (2 * N * c1 )
    //Rl=-Rl;
    //new_ray=Ray(P,normalise(Rl))
    //return trace_ray(ray,..)
    if(object.getReflection().max() < EPS) return COLOR_BLACK;
    Point3d rayDir = ray.D().normalized();
    Color3d reflectionCoeff;
    Color3d average_color=Color3d(0.0,0.0,0.0);
    if(isCritical)
    {
        reflectionCoeff = COLOR_WHITE;
    }
    else
    {
        reflectionCoeff = object.getReflection();
    }
    Vector3d Rl = (rayDir - ((N.normalized()|rayDir) * N.normalized())*2).normalize();
    Ray newRay = Ray(P, Rl);

    if(_numberOfRefRays == 1)
    {
        return reflectionCoeff * trace_ray(newRay, vis);
    }
    else {
        //send multiple rays and average
        for (int i; i<_numberOfRefRays;i++) {
            average_color+=trace_ray(perturbateRay(newRay),vis);
        }
        average_color*=(1.0/_numberOfRefRays);
        return object.getReflection()*average_color;
    }

}

Color3d Scene::calcRefraction(const Ray& ray, const Point3d& P, const Vector3d& N, const Object& object, double vis /*= 1.0*/) const {
    //snell's law
    //ToDO: is _index is n_1/n_2?
    //Todo: where should we take 1/index?
    if(object.getTransparency().max() < EPS) return  COLOR_BLACK;
    double index = object.getIndex();
    Point3d rayDir = ray.D().normalized(), n;
    double cos_theta_i = (N|rayDir);
    {
        if(cos_theta_i > EPS && cos_theta_i < 1 - EPS)
        {
            index = index / INDEX_VOID;
            n = -N;
        }
        else
        {
            index = INDEX_VOID/index;
            n = N;
        }
    }
    cos_theta_i = (n|rayDir) * index;
    double cos_theta_t = 1 - pow(index,2) * (1 - (n | rayDir) * (n | rayDir));
    Color3d average_color=Color3d(0.0,0.0,0.0);
    if(cos_theta_t < EPS)
    {
        return object.getTransparency() * calcReflection(ray, P, n, object, vis, true);
    }
    Vector3d Re = (cos_theta_i - sqrt(cos_theta_t)) * N - rayDir * index;
    Ray newRay = Ray(P, Re);
    if(_numberOfRefRays == 1)
    {
        return object.getTransparency() * trace_ray(newRay, vis);
    }

    else {
        //send multiple rays and average
        for (int i; i<_numberOfRefRays;i++) {
            average_color+=trace_ray(perturbateRay(newRay),vis);
        }
        average_color*=(1.0/_numberOfRefRays);
        return object.getTransparency()*average_color;
    }
}

Ray Scene::perturbateRay(const Ray& r) const {
    //http://www.mathworks.com/matlabcentral/answers/72631-create-orthonormal-basis-from-a-given-vector
    //create an orthonormal basis.
    Vector3d ray_dir=r.D();
    Vector3d x_1=Vector3d(0.0,-ray_dir[2],ray_dir[1]).normalize();
    Vector3d x_2=(ray_dir%x_1).normalize();
    //send out the cone 1 unit from r.p(), so the max radial distance deviating from the centre of the cone is max
    //defined below.
    double max=sin(_cutoffAngle*PI/PI_DEGREES);
    //2 processes. Randomise magnitude of x_1 and x_2. Then randomise their positivity/negativity.
    x_1*=fmod((double)rand(),max);
    x_2*=fmod((double)rand(),max);
    double pos_check_1=fmod((double)rand(),2);
    double pos_check_2=fmod((double)rand(),2);

    if (pos_check_1>1) x_1*=-1;
    if (pos_check_2>1) x_1*=-1;

    return Ray(r.O(),ray_dir+x_1+x_2);
}


Point3d Scene::sphereLightRandomRay(const SphereLight& light) const
{
    //x=Rcos(theta)cos(phi), y=Rcos(theta)sin(phi), z=Rsin(theta)
    double r = light._radius;
    Point3d center = light._position;
    double z = fmod(rand(),r * 2) - r;
    double phi = fmod(rand(), (2 * PI));
    double theta = asin(z/r);
    double x = r * cos(theta) * cos(phi);
    double y = r * cos(theta) * sin(phi);
    return light._position + Point3d(x, y, z);
}




