#pragma once
#include "Vertex.h"

class Triangle;

class Ray
{
	public:
		Ray(Vertex in_start, vec3 dir, dvec3 in_color);
		~Ray() = default;
		friend ostream& operator<<(ostream& out, Ray r);
		
		dvec3 color = vec3(0, 0, 0);
		Vertex start;
		Vertex end;
		vec3 dir;
		Triangle *tri = nullptr;

	private:
};

