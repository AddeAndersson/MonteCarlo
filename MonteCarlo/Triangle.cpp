#include "Triangle.h"



Triangle::Triangle(std::vector<Vertex> &in_vertices, glm::vec3 in_colorDbl)
:vertices(in_vertices), colorDbl(in_colorDbl)
{
	glm::vec3 v0 = vertices[0].pos;
	glm::vec3 v1 = vertices[1].pos;
	glm::vec3 v2 = vertices[2].pos;

	normal = normalize(cross((v1 - v0),v2-v0));
}

glm::vec3 Triangle::rayIntersection(Ray &arg)
{
	glm::vec3 v0 = vertices[0].pos;
	glm::vec3 v1 = vertices[1].pos;
	glm::vec3 v2 = vertices[2].pos;

	glm::vec3 T = glm::vec3(arg.start.pos) - v0;
	glm::vec3 E1 = v1 - v0;
	glm::vec3 E2 = v2 - v0;
	glm::vec3 D = arg.end.pos - arg.start.pos;
	glm::vec3 P = glm::cross(D,E2);
	glm::vec3 Q = glm::cross(T, E1);

	glm::vec3 res;

	res.x = glm::dot(Q,E2) / glm::dot(P,E1);
	res.y = glm::dot(P, T) / glm::dot(P, E1);
	res.z = glm::dot(Q, D) / glm::dot(P, E1);

	//t = x
	//u = y
	//v = z 
	return res;

}

Triangle& Triangle::operator=(Triangle lhs)
{
	std::swap(vertices, lhs.vertices);
	std::swap(colorDbl, lhs.colorDbl);
	std::swap(normal, lhs.normal);

	return *this;
}
