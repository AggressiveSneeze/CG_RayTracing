//
// Created by jadams on 1/17/16.
//

#include "object.h"s

// Original methods (i left them because the ex3 scene uses them  //
// and the ex mentioned that we must work with your ex3 file)     //
Color3d& diffuse();
Color3d& specular();
Color3d& ambient();
Color3d& reflection();
Color3d& transparency();
double& index();
double& shining();

// Const get methods (to let the compiler do the optimizations we want it to do)  //
const Object::Color3d&  getDiffuse() {
    return _diffuse;
}
const Object::Color3d&  getSpecular()   {
    return _specular;

}
const Object::Color3d&  getAmbient() {
    return _ambient;
}

const Object::Color3d&  getReflection() {
    return _reflection;
}
const Object::Color3d&  getTransparency() {
    return _transparency;
}
const Object::double    getIndex()  {
    return _index;
}
const Object::double    getShining() {
    return _shining;
}