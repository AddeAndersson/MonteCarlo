#include "Triangle.h"



Triangle::Triangle(glm::vec3 in_vertices, glm::vec3 in_color, glm::vec3 in_normal)
:vertices(in_vertices), color(in_color), normal(in_normal)
{
}

void Triangle::rayIntersection(Ray arg)
{
	
}

Triangle& Triangle::operator=(Triangle lhs)
{
	std::swap(vertices, lhs.vertices);
	std::swap(color, lhs.color);
	std::swap(normal, lhs.normal);

	return *this;
}

Triangle::~Triangle()
{
}
