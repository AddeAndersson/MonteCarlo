#include "Vertex.h"



Vertex::Vertex(double in_x, double in_y, double in_z)
{
	glm::vec4 pos = glm::vec4(in_x, in_y, in_z, 1.0);
}

Vertex& Vertex::operator=(Vertex lhs)
{
	std::swap(pos, lhs.pos);

	return *this;
}

Vertex::~Vertex()
{
}
