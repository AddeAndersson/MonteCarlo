#include "Scene.h"
#include <iomanip>
#include <iostream>


Scene::Scene(std::vector<Triangle> in_triangles)
{
	for(auto i = 0; i < in_triangles.size(); ++i)
	{
		triangles->push_back(in_triangles[i]);
	}
}


using namespace std;
int main()
{
	//Room

	//Sphere

	//Cube

	//Tetrahedron

	return 0;
}