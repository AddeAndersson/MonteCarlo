#pragma once
#include <glm/glm.hpp>

class Vertex
{
	public:
		Vertex(double in_x, double in_y, double in_z);
		~Vertex();
	
		Vertex& operator=(Vertex lhs);
	
		glm::vec4 pos;
	private:
};

