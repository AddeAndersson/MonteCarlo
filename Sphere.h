#pragma once
#include "Scene.h"
#include "Surface.h"

class Sphere
{
	public:
		Sphere(vec3 in_center, dvec3 in_color, float in_radius, Surface in_surface);
		Sphere();
		friend ostream &operator<<(ostream &out, Sphere &s);
		~Sphere() = default;

		vec3 center;
		dvec3 colorDbl;
		float radius;
		Surface surface;
};

bool rayIntersectionSphere(Ray &r, Sphere &sphere);