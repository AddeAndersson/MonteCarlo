#include "Vertex.h"



Vertex::Vertex(double in_x, double in_y, double in_z)
{
	glm::vec3 xyz = glm::vec3(in_x, in_y, in_z);
}


Vertex::~Vertex()
{
}
