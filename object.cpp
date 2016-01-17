//
// Created by jadams on 1/17/16.
//

#include "object.h"

//constructors
Object::Object() {};

//texture stuff
void Object::set_texture_map(BImage* image){};

// Original methods (i left them because the ex3 scene uses them  //
// and the ex mentioned that we must work with your ex3 file)     //

Color3d& Object::diffuse() {
    return _diffuse;
}

Color3d& Object::specular() {
    return _specular;
}
Color3d& Object::ambient() {
    return _ambient;
}
Color3d& Object::reflection() {
    return _reflection;
}
Color3d& Object::transparency() {
    return _transparency;
}
double& Object::index() {
    return _index;
}
double& Object::shining() {
    return _shining;
}

// Const get methods (to let the compiler do the optimizations we want it to do)  //
const Color3d&  Object::getDiffuse() const {
    return _diffuse;
}
const Color3d&  Object::getSpecular()  const {
    return _specular;

}
const Color3d&  Object::getAmbient() const {
    return _ambient;
}

const Color3d&  Object::getReflection() const {
    return _reflection;
}
const Color3d&  Object::getTransparency() const {
    return _transparency;
}
const double    Object::getIndex() const  {
    return _index;
}
const double    Object::getShining() const {
    return _shining;
}