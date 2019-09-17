#include "Direction.h"
#include <valarray>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

Direction::Direction(double in_x, double in_y, double in_z)
:x(in_x), y(in_y), z(in_z)
{
	glm::vec3 Dir = glm::vec3(x,y,z);
}

void Direction::Normalize()
{
	x = x / (std::abs((x + y + z)));
	y = y / (std::abs((x + y + z)));
	z = z / (std::abs((x + y + z)));
}

Direction::~Direction()
{
}
