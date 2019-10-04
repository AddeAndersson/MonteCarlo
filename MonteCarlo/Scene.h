#pragma once
#include "Camera.h"

class Sphere;

class Scene
{
public:
	Scene(vector<Triangle> in_triangles);
	~Scene() = default;

	vector<Triangle> triangles;
	Sphere *sphere = nullptr;
};

