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
		Vertex(float in_x, float in_y, float in_z);
		~Vertex() = default;
	
		Vertex& operator=(Vertex lhs);
		friend ostream &operator<<(ostream &out, Vertex &v);
	
		vec4 pos;
	private:
};

void print(vec3 &v);
void print(vec4 &v);
void print(dvec3 &v);
