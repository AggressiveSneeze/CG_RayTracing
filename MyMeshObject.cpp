//
// Created by shirelga on 1/11/16.
//

#include "MyMeshObject.h"
#include "MyMesh.h"
#include "sphere.h"
MyMeshObject::MyMeshObject(MyMesh & mesh): _mesh(mesh)) {

	//maybe something here to compute bounding sphere and centre.
	//is it really necessary? Can't just check if ray intersects with any of the triangles?

	//TODO: What's wrong with this call?
	calculateBoundingSphere();

}


int intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, 
								OUT Vector3d& N, OUT Color3d& texColor) {

	//check every triangle in the mesh using the bookmarked algo.
	//normal can be computed by accessing the particular face?(does this exist in openmesh?)
	//Is that it?

};

void MyMeshObject::calculateBoundingSphere() {

	Vector3d center;
	Vector3d xmin,xmax,ymin,ymax,zmin,zmax;
	xmin=ymin=zmin=Vector3d(1.0,1.0,1.0)*INF;
	xmax=ymin=zmin=Vector3d(-1.0,-1.0,-1.0)*INF;

	for (MyMesh::VertexIter v_it=_mesh.vertices_begin();
					v_it!=_mesh.vertices_end(); ++v_it) {
		if (v_it.x < xmin.x) xmin = _mesh.point(v_it);
		if (v_it.x > xmax.x) xmax = _mesh.point(v_it);
		if (v_it.y < ymin.y) ymin = _mesh.point(v_it);
		if (v_it.y > ymax.y) ymax = _mesh.point(v_it);
		if (v_it.z < zmin.z) zmin = _mesh.point(v_it);
		if (v_it.z > zmax.z) zmax = _mesh.point(v_it);
	}

	float x_span=(xmax-xmin).sqrnorm();
	float y_span=(ymax-ymin).sqrnorm();
	float z_span=(zmax-zmin).sqrnorm();
	Vector3d dia1 = xmin;
	Vector3d dia2 = xmax;
	float max_span=x_span;
	if (y_span > max_span)
	{
		max_span = y_span;
		dia1 = ymin; dia2 = ymax;
	}
	if (z_span > max_span)
	{
		dia1 = zmin; dia2 = zmax;
	}
	center = (dia1 + dia2) * 0.5f;

	float sqRad = (dia2 - center).sqrnorm();
	float radius = sqrtf(sqRad);

	float d,r,offset;
	for (MyMesh::VertexIter v_it=_mesh.vertices_begin();
		 v_it!=_mesh.vertices_end(); ++v_it) {
		d=(_mesh.point(v_it)-center).sqrnorm();
		if(d>sqRad) {
			r=sqrtf(d);
			radius=(radius+r)*0.5f;
			sqRad=radius*radius;
			offset=r-radius;
			//TODO:is this line okay using the defined operators?
			center=(radius*center+offset*_mesh.point(v_it))/r;
		}
	}
	//now we have center and radius.
	//assign the bounding sphere
	*_boundingSphere=Sphere(center,radius);
	//Todo: can the centre just be:
	_center=center;
	//ported from
	//http://answers.unity3d.com/questions/836915/smallest-bounding-sphere.html

}


int MyMeshObject::intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor) {

	//check bounding sphere first. early exit if ray doesn't even go through bounding sphere.
	if(!_boundingSphere->intersect(ray,tMax,t,P,N,texColor)) return 0;
	//psuedo code:
	//for every face
	//check intersection between ray and triangle.
	//have to go through every face in case there is a closer one.






}
