#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <cmath>

using namespace glm;
using namespace std;

class Vertex
{
	public:
		Vertex(double in_x, double in_y, double in_z);
		~Vertex() = default;
	
		Vertex& operator=(Vertex lhs);
		friend ostream &operator<<(ostream &out, Vertex &v);
	
		dvec4 pos;
	private:
};

void print(dvec3 &v);
void print(dvec4 &v);
