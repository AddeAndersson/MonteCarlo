#pragma once
#include "Ray.h"
#include "Surface.h"

const int DIFFUSE = 0;
const int MIRROR = 1;
const int TRANSPARENT = 2;
const int LIGHT = 3;

class Triangle
{
	public:
		Triangle();
		Triangle(vector<Vertex> &in_vertices, dvec3 in_colorDbl, Surface in_surface);
		bool rayIntersection(Ray &arg); //Return type might be wrong
		Triangle& operator=(Triangle lhs);
		friend ostream& operator<<(ostream &out, Triangle t);

		~Triangle() = default;
	
		vector<Vertex> vertices;
		dvec3 colorDbl;
		vec3 normal;
		Surface surface;
};

void loadMeshToScene(vector<Triangle> &triangles, string filename, dvec3 in_color, Surface in_surface);