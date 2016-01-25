#include "polygon.h"

//constructors

Polygon::Polygon(vector<Point3d> & vertices): _vertices(vertices) {//find the triangles
    triangulate();
};

Polygon::Polygon(vector<Point3d> & vertices, Vector3d & normal): _vertices(vertices), _normal(normal){
    //find the triangles
    triangulate();
};

// Constructor - Create a polygon from the given vertices, using the given texture map coordinates //
Polygon::Polygon(vector<Point3d> & vertices, vector<Point2d> textices):_vertices(vertices),_textices(textices) {
    //find the triangles
    triangulate();
};

// Constructor - Create a polygon from the given vertices, using the given normal and texture map coordinates //
Polygon::Polygon(vector<Point3d> & vertices, vector<Point2d> textices, Vector3d & normal):_vertices(vertices),
                                            _textices(textices),_normal(normal){
    //find the triangles
    triangulate();
};
//deconstructor
Polygon::~Polygon(){};


void Polygon::triangulate() {
    for (int i=1;i<_vertices.size()-1;i++) {
        _triangles.push_back(new Triangle(_vertices[0],_vertices[i],_vertices[i+1]));
    }
}


int Polygon::intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor) {

    bool intersection=false;
    float min_t=INF;
    double temp_t;
    Point3d temp_P;
    Vector3d temp_N;

    //check intersection for every triangle
    for(auto triangle:_triangles) {
        if (triangle->intersect(ray,tMax,temp_t,temp_P,temp_N)) {
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
    if (intersection) return 1;
    else return 0;
};
