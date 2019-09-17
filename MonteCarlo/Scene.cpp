#include "Scene.h"
#include <iomanip>
#include <iostream>


Scene::Scene(std::vector<Triangle> in_triangles)
{
	for(int i = 0; i < in_triangles.size(); ++i)
	{
		triangles[i] = in_triangles[i];
	}
}


Scene::~Scene()
{
}


using namespace std;
int main()
{
	cout << "Hej" << endl;
	return 0;
}