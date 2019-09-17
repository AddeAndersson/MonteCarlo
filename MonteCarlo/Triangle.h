#pragma once
#include <glm/glm.hpp>
#include "Ray.h"

class Triangle
{
	public:
		Triangle(glm::vec3 in_vertices, glm::vec3 in_color, glm::vec3 in_normal);
		void rayIntersection(Ray arg); //Return type might be wrong
		Triangle& operator=(Triangle lhs);
		~Triangle();
	
		glm::vec3 vertices;
		glm::vec3 color;
		glm::vec3 normal;
};

