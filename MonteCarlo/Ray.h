#pragma once
#include <glm/glm.hpp>
#include "Triangle.h"
#include "Vertex.h"

class Ray
{
	public:
		Ray(Vertex in_start, Vertex in_end, glm::vec3 in_color);
		~Ray() = default;
		
		glm::vec3 color;
		Vertex start;
		Vertex end;
		Triangle* tri;

	private:
};

