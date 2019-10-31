#include "Sphere.h"
#include "Triangle.h"


Sphere::Sphere(vec3 in_center, dvec3 in_color, float in_radius, Surface in_surface)
:center(in_center), colorDbl(in_color), radius(in_radius), surface(in_surface)
{

}

Sphere::Sphere()
:center(0.0f), colorDbl(dvec3(0.0, 0.0, 0.0)), radius(0.1f), surface(DIFFUSE)
{
	
}


bool rayIntersectionSphere(Ray &r, Sphere &sphere)
{
	float b = dot(2.0f * r.dir, vec3(r.start.pos) - sphere.center);
	float a = 1.0f;
	float c = dot(vec3(r.start.pos) - sphere.center, vec3(r.start.pos) - sphere.center) - pow(sphere.radius, 2.0f);
	
	//no intersection with sphere
	if(pow(b * 0.5f, 2.0f) - a * c < EPS)
	{
		return false;
	}
	
	float d1 = -(b * 0.5f) + sqrt(pow(b * 0.5f, 2.0f) - a * c);
	float d2 = -(b * 0.5f) - sqrt(pow(b * 0.5f, 2.0f) - a * c);

	//no legitimate intersection with sphere
	if (d1 < EPS && d2 < EPS) return false;

	//d2 is always less than d1, so if d2 > 0.0, so is d1
	float d = (d2 > EPS) ? d2 : d1;
	if (sphere.surface.type != TRANSPARENT && d2 < EPS) return false;
	//if (d2 < EPS) return false; //disallows bouncing inside sphere

	r.end.pos = vec4(vec3(r.start.pos) + r.dir * d, 1.0f);
	
	return true;	
}

ostream &operator<<(ostream &out, Sphere &s)
{
	out << "Sphere: \n"; 
	out << "\t" << "Center: ";  print(s.center); out << endl;
	out << "\t" << "Color: "; print(s.colorDbl); out << endl;
	out << "\t" << "Radius: " << s.radius << endl;

	return out;
}