#pragma once
#include "Ray.h"
//#include "Triangle.h"

class Pixel
{
public:
	Pixel(dvec3 in_colorDbl, Ray *in_ray);
	Pixel(dvec3 in_colorDbl);

	friend ostream& operator<<(ostream &out, Pixel p);
	~Pixel() = default;

	dvec3 colorDbl;
	Ray *ray;
};

