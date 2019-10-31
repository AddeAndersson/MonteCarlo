#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
#include <random>

class Scene;
using namespace glm;


class Surface
{
public:
	Surface(int in_type);
	Surface(int in_type, float in_roughness, double in_albedo);
	float resolveDiffuse(Scene &sc, Ray &r, int numberOfShadowRays, vec3 in_normal);
	~Surface() = default;

	int type;
	double absorption = 0.5;
	float roughness; //Standard deviation from gaussian distribution function for Oren-Nayar
};

