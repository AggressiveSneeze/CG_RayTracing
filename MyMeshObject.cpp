//
// Created by shirelga on 1/11/16.
//

#include "MyMeshObject.h"
#include "MyMesh.h"
#include "sphere.h"
#include <vector>
MyMeshObject::MyMeshObject(MyMesh & mesh){

	_mesh=mesh;

	//maybe something here to compute bounding sphere and centre.
	//is it really necessary? Can't just check if ray intersects with any of the triangles?

	//TODO: What's wrong with this call?
	calculateBoundingSphere();

}


void MyMeshObject::calculateBoundingSphere() {

	Vector3d center;
	//Vector3d xmin,xmax,ymin,ymax,zmin,zmax;
	//TODO: Is this the right way?
	Vector3d xmin(INF,INF,INF);
	Vector3d ymin(INF,INF,INF);
	Vector3d zmin(INF,INF,INF);
	Vector3d xmax(-INF,-INF,-INF);
	Vector3d ymax(-INF,-INF,-INF);
	Vector3d zmax(-INF,-INF,-INF);

	for (MyMesh::VertexIter v_it=_mesh.vertices_begin();
					v_it!=_mesh.vertices_end(); ++v_it) {
		if (_mesh.point(v_it)[0] < xmin[0]) xmin = _mesh.point(v_it);
		if (_mesh.point(v_it)[0] > xmax[0]) xmax = _mesh.point(v_it);
		if (_mesh.point(v_it)[1] < ymin[1]) ymin = _mesh.point(v_it);
		if (_mesh.point(v_it)[1] > ymax[1]) ymax = _mesh.point(v_it);
		if (_mesh.point(v_it)[2] < zmin[2]) zmin = _mesh.point(v_it);
		if (_mesh.point(v_it)[2] > zmax[2]) zmax = _mesh.point(v_it);
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
			center=(center*radius+_mesh.point(v_it)*offset)/r;
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

//TODO: Refactor to use method within triangle object

int MyMeshObject::intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N,
																				OUT Color3d& texColor) {

	//check bounding sphere first. early exit if ray doesn't even go through bounding sphere.
	if(!_boundingSphere->intersect(ray,tMax,t,P,N,texColor)) return 0;
	//psuedo code:
	//for every face
	//check intersection between ray and triangle.
	//have to go through every face in case there is a closer one.
	float min_t=INF;
	bool intersection=false;
	MyMesh::FaceHandle fHandle;
	double temp_t;
	Point3d temp_P;
	Vector3d temp_N;
	//make sure mesh gets normals.
	_mesh.request_face_normals();
	_mesh.update_face_normals();
	for (MyMesh::FaceIter f_it=_mesh.faces_begin(); f_it!=_mesh.faces_end(); ++f_it) {
		//get handle of current face
		fHandle=f_it.handle();
		//check triangle, why can't this method be accessed?
		if (check_triangle(fHandle,ray,tMax,temp_t,temp_P,temp_N)) {
			if (temp_t<min_t) {
				//upgrade our local closest
				min_t=temp_t;
				//upgrade our externals
				t=temp_t;
				P=temp_P;
				N=temp_N;
				if (!intersection) intersection=true;
			}
		}
	}
	_mesh.release_face_normals();
	if (intersection) return 1;
	else return 0;
}

int MyMeshObject::check_triangle(IN MyMesh::FaceHandle fhandle, IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N) {

	//get the vertices of triangle:
	MyMesh::FaceVertexIter fvIter;
	//MyMesh::VertexHandle vHandle;
	std::vector<Point3d> verts;
	//ported from http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
	//TODO: is this okay?
	for (fvIter=_mesh.fv_iter(fhandle);fvIter;++fvIter) {
		verts.push_back(_mesh.point(fvIter));
	}
	//find vectors for two edges sharing vert0
	Vector3d edge1=verts[1]-verts[0],edge2=verts[2]-verts[0];
	//begin calculating determined - also used to calculate U parameter.
	Vector3d pvec=ray.D()%edge2; //cross product
	//if determinant is near zero, ray lies in plane of triangle.
	float det=edge1|pvec;
	//not sure how to determine which side of triangle is inside/outside mesh, so using non culling branch for the time
	//being.
	if (det > -EPS && det<EPS) return 0;

	float inv_det=1.0/det;

	//calculate distance from vert0 to ray origin.
	Vector3d tvec=ray.O()-verts[0];
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
	P=verts[0]*(1-u-v)+verts[1]*u+verts[2]*v;
	//TODO: Think about this. This is the face normal. Is it okay as the normal from the point since the triangle
	//TODO:																				is a slice of the same plane?
	N=_mesh.normal(fhandle);
	return 1;

}
