#include "Triangle.h"

const double EPS = 1e-4;

Triangle::Triangle()
{
	vertices.push_back(Vertex(0.0, 0.0, 0.0));
	vertices.push_back(Vertex(0.0, 0.0, 0.0));
	vertices.push_back(Vertex(0.0, 0.0, 0.0));
	colorDbl = dvec3(1.0, 1.0, 1.0);
	surface = OREN;
	normal = dvec3(0.0, 0.0, 0.0);

}


Triangle::Triangle(vector<Vertex> &in_vertices, vec3 in_colorDbl, int in_surface)
:vertices(in_vertices), colorDbl(in_colorDbl), surface(in_surface)
{
	vec3 v0 = vertices[0].pos;
	vec3 v1 = vertices[1].pos;
	vec3 v2 = vertices[2].pos;

	normal = normalize(cross(v1 - v0,v2-v0));
}

//möller-trumbore algorithm
bool Triangle::rayIntersection(Ray &arg)
{
	dvec3 v0 = vertices[0].pos;
	dvec3 v1 = vertices[1].pos;
	dvec3 v2 = vertices[2].pos;

	dvec3 T = dvec3(arg.start.pos) - v0; //T
	dvec3 E1 = v1 - v0; //E1
	dvec3 E2 = v2 - v0; // E2
	dvec3 D = arg.dir; //D
	dvec3 P = cross(D,E2); //P
	dvec3 Q = cross(T, E1); 
	double a = dot(E1, P);

	//Ray is parallel to triangle
	if (a > -EPS && a < EPS) return false;

	double t, u, v;
	u = dot(P, T) / a; if (u < 0.0 || u > 1.0) return false;
	v = dot(Q, D) / a; if (v < 0.0 || u + v > 1.0 || v > 1.0) return false;
	t = dot(Q, E2) / a;

	//Ray intersection
	if(t > EPS)
	{
		arg.end.pos = dvec4(dvec3(arg.start.pos) + arg.dir * t, 1.0);
		return true;
	}

	return false;
}

Triangle& Triangle::operator=(Triangle lhs)
{
	swap(vertices, lhs.vertices);
	swap(colorDbl, lhs.colorDbl);
	swap(normal, lhs.normal);
	swap(surface, lhs.surface);

	return *this;
}

ostream& operator<<(ostream &out, Triangle t)
{
	out << "Triangle: \n";
	
	for(int i = 0; i < 3; ++i)
	{
		out << "\tVertex " << i << ": " << t.vertices[i];
	}

	out << "\t" << "Normal: ";  print(t.normal); out << endl;
	out << "\t" << "Color: "; print(t.colorDbl); out << endl;
	return out;
}