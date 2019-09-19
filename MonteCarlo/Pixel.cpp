#include "Pixel.h"



Pixel::Pixel(glm::vec3 in_colorDbl = glm::vec3(1.0,1.0,1.0), Ray *in_ray)
:colorDbl(in_colorDbl), ray(in_ray)
{
}


Pixel::Pixel(glm::vec3 in_colorDbl)
:colorDbl(in_colorDbl)
{
	ray = nullptr;
}
