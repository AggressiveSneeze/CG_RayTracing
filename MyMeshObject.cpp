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

void calculateBoundingSphere() {

	Vector3d center;
	float radius;
	Vector3d xmin,xmas,ymin,ymax,zmin,zmax;
	xmin=ymin=zmin=Vector3d(1,1,1)*INF;
	xmax=ymin=zmin=Vector3d(-1,-1,-1)*INF;

	for (MyMesh::VertexIter v_it=mesh.vertices_begin(); 
					v_it!=mesh.vertices_end(); ++v_it) {
		if (v_it.x<xmin.x) xmin=v_it;
		if (v_it.x>xmax.x) xmax=v_it;
		if (v_it.y<ymin.y) ymin=v_it;
		if (v_it.y>ymax.y) ymax=v_it;
		if (v_it.z<zmin.z) zmin=v_it;
		if (v_it.z>zmax.z) zmax=v_it;
	}
	

	//  public struct BoundingSphere
 // {
 //     public Vector3 center;
 //     public float radius;
 //     public BoundingSphere(Vector3 aCenter, float aRadius)
 //     {
 //         center = aCenter;
 //         radius = aRadius;
 //     }
 
 //     public static BoundingSphere Calculate(IEnumerable<Vector3> aPoints)
 //     {
 //         Vector3 xmin, xmax, ymin, ymax, zmin, zmax;
 //         xmin = ymin = zmin = Vector3.one * float.PositiveInfinity;
 //         xmax = ymax = zmax = Vector3.one * float.NegativeInfinity;
 //         foreach(var p in aPoints)
 //         {
 //             if(p.x < xmin.x) xmin = p;
 //             if(p.x > xmax.x) xmax = p;
 //             if(p.y < ymin.y) ymin = p;
 //             if(p.y > ymax.y) ymax = p;
 //             if(p.z < zmin.z) zmin = p;
 //             if(p.z > zmax.z) zmax = p;
 //         }
			//magnitude squared. 
 //         var xSpan = (xmax - xmin).sqrMagnitude;
 //         var ySpan = (ymax - ymin).sqrMagnitude;
 //         var zSpan = (zmax - zmin).sqrMagnitude;
 //         var dia1 = xmin;
 //         var dia2 = xmax;
 //         var maxSpan = xSpan;
 //         if (ySpan > maxSpan)
 //         {
 //             maxSpan = ySpan;
 //             dia1 = ymin; dia2 = ymax;
 //         }
 //         if (zSpan > maxSpan)
 //         {
 //             dia1 = zmin; dia2 = zmax;
 //         }
 //         var center = (dia1 + dia2) * 0.5f;
 //         var sqRad = (dia2 - center).sqrMagnitude;
 //         var radius = Mathf.Sqrt(sqRad);
 //         foreach (var p in aPoints)
 //         {
 //             float d = (p - center).sqrMagnitude;
 //             if(d > sqRad)
 //             {
 //                 var r = Mathf.Sqrt(d);
 //                 radius = (radius + r) * 0.5f;
 //                 sqRad = radius * radius;
 //                 var offset = r - radius;
 //                 center = (radius * center + offset * p) / r;
 //             }
 //         }
 //         return new BoundingSphere(center, radius);
 //     }
 // }

}
