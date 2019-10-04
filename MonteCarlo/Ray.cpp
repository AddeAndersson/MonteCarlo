#include "Ray.h"

Ray::Ray(Vertex in_start, dvec3 in_dir, dvec3 in_color)
:start(in_start), dir(normalize(in_dir)), color(in_color), end(in_start)
{
}

ostream& operator<<(ostream& out, Ray r)
{
	out << "Ray: " << endl;
	out << "\tStart: " << r.start;
	out << "\tEnd: " << r.end;
	out << "\tDirection"; print(r.dir); out << endl;
	out << "\tColor: "; print(r.color);

	return out;
}