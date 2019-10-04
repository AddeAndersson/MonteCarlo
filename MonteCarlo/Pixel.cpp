#include "Pixel.h"



Pixel::Pixel(dvec3 in_colorDbl, Ray *in_ray)
:colorDbl(in_colorDbl), ray(in_ray)
{
}


Pixel::Pixel(dvec3 in_colorDbl)
:colorDbl(in_colorDbl)
{
	ray = nullptr;
}


ostream& operator<<(ostream &out, Pixel p)
{
	out << "Pixel: "; print(p.colorDbl); cout << endl;
	return out;
}
