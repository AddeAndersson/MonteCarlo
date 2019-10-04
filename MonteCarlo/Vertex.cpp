#include "Vertex.h"

Vertex::Vertex(double in_x, double in_y, double in_z)
{
	pos = dvec4(in_x, in_y, in_z, 1.0);
}

Vertex& Vertex::operator=(Vertex lhs)
{
	swap(pos, lhs.pos);

	return *this;
}

ostream&  operator<<(ostream &out, Vertex &v)
{
	out << "Vertex: "; print(v.pos); out << endl;
	return out;
}

void print(dvec3 &v)
{
	cout << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

void print(dvec4 &v)
{
	cout << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

