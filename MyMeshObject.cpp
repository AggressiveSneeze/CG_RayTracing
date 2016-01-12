//
// Created by shirelga on 1/11/16.
//

#include "MyMeshObject.h"

MyMeshObject::MyMeshObject(MyMesh & mesh): _mesh(mesh)) {

	//maybe something here to compute bounding sphere and centre.
	//is it really necessary? Can't just check if ray intersects with any of the triangles?

};


int intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, 
								OUT Vector3d& N, OUT Color3d& texColor) {

	//check every triangle in the mesh using the bookmarked algo.
	//normal can be computed by accessing the particular face?(does this exist in openmesh?)
	//Is that it?

	

};


