#pragma once
#include "Scene.h"

class Sphere
{
	public:
		Sphere(dvec3 in_center, dvec3 in_color, double in_radius, int in_surface);
		friend ostream &operator<<(ostream &out, Sphere &s);
		~Sphere() = default;

		dvec3 center;
		dvec3 colorDbl;
		double radius;
		int surface;
};

bool rayIntersectionSphere(Ray &r, Sphere sphere);