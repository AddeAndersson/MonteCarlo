#pragma once
#include "Triangle.h"
#include <vector>

class Scene
{
public:
	Scene(Triangle in_triangles[]);
	~Scene();

private:
	std::vector<Triangle> triangles[24];
};

