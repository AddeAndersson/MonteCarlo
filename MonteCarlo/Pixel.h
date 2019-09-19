#pragma once
#include <glm/vec3.hpp>
#include "Ray.h"

class Pixel
{
public:
	Pixel(glm::vec3 in_colorDbl, Ray *in_ray);
	Pixel(glm::vec3 in_colorDbl);
	~Pixel() = default;

	glm::vec3 colorDbl;
	Ray *ray;
};

