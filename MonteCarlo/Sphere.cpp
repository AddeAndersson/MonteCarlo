#include "Sphere.h"


Sphere::Sphere(dvec3 in_center, dvec3 in_color, double in_radius, int in_surface)
:center(in_center), colorDbl(in_color), radius(in_radius), surface(in_surface)
{

}


bool rayIntersectionSphere(Ray &r, Sphere sphere)
{
	double b = dot(2.0 * r.dir, dvec3(r.start.pos) - sphere.center);
	double a = 1.0;
	double c = dot(dvec3(r.start.pos) - sphere.center, dvec3(r.start.pos) - sphere.center) - pow(sphere.radius, 2.0);
	
	//no intersection with sphere
	if(pow(b * 0.5, 2.0) - a * c < EPS)
	{
		return false;
	}
	
	double d1 = -(b * 0.5) + sqrt(pow(b * 0.5, 2.0) - a * c);
	double d2 = -(b * 0.5) - sqrt(pow(b * 0.5, 2.0) - a * c);

	//no legitimate intersection with sphere
	if (d1 < EPS && d2 < EPS) return false;

	//d2 is always less than d1, so if d2 > 0.0, so is d1
	double d = (d2 > EPS) ? d2 : d1;

	double offset = length((dvec3(r.start.pos) + r.dir * d) - sphere.center) - sphere.radius;

	r.end.pos = dvec4(dvec3(r.start.pos) + r.dir * d, 1.0);

	return true;	
}

ostream &operator<<(ostream &out, Sphere &s)
{
	out << "Sphere: \n"; 
	out << "\t" << "Center: ";  print(s.center); out << endl;
	out << "\t" << "Colo: "; print(s.colorDbl); out << endl;
	out << "\t" << "Radius: " << s.radius << endl;

	return out;
}