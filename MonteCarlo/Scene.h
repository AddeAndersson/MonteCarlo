#pragma once
#include "Triangle.h"
#include <vector>

class Scene
{
public:
	Scene(std::vector<Triangle> in_triangles);
	~Scene() = default;

private:
	std::vector<Triangle> triangles[24];
};

