#pragma once
#include "Ray.h"

const int LAMBERTIAN = 0;
const int OREN = 1;
const int MIRROR = 2;
const int TRANSPARENT = 3;
const int LIGHT = 4;

class Triangle
{
	public:
		Triangle();
		Triangle(vector<Vertex> &in_vertices, vec3 in_colorDbl, int in_surface);
		bool rayIntersection(Ray &arg); //Return type might be wrong
		Triangle& operator=(Triangle lhs);
		friend ostream& operator<<(ostream &out, Triangle t);

		~Triangle() = default;
	
		vector<Vertex> vertices;
		dvec3 colorDbl;
		dvec3 normal;
		int surface;
};

