#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
#include "Vertex.h"
#include <vector>

class Triangle
{
	public:
		Triangle(std::vector<Vertex> &in_vertices, glm::vec3 in_colorDbl);
		glm::vec3 rayIntersection(Ray &arg); //Return type might be wrong
		Triangle& operator=(Triangle lhs);
		~Triangle() = default;
	
		std::vector<Vertex> vertices;
		glm::vec3 colorDbl;
		glm::vec3 normal;
};

