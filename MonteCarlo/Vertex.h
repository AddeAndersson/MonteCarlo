#pragma once
#include <glm/glm.hpp>

class Vertex
{
	public:
		Vertex(double in_x, double in_y, double in_z);
		~Vertex();
	

	private:
		glm::vec3 xyz;
		//glm::vec3 homogenous;
};

