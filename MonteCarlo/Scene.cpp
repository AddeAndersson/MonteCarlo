#include "Scene.h"
#include "Triangle.h"
#include "Sphere.h"

Scene::Scene(vector<Triangle> in_triangles)
{
	for (auto &t : in_triangles) triangles.push_back(t);
}


using namespace std;
int main()
{
	// View from above:
	// 
	//   y
	//   ^
	//	 |
	//   o--->x
	//	 z
	//
	//     b-----c
	//    /       \
	//   a         d
	//    \       /
	//     f-----e

	vector<Triangle> sceneTri;

	//Colors
	dvec3 white(1.0, 1.0, 1.0);
	dvec3 red(1.0, 0.0, 0.0);
	dvec3 yellow(1.0, 1.0, 0.2);
	dvec3 cyan(0.2, 1.0, 1.0);
	dvec3 green(0, 1, 0);
	dvec3 blue(0, 0, 1);

	//Sphere
	Sphere *S1 = new Sphere(dvec3(8.0, -3.0, 0.0), cyan, 2.0, MIRROR);
	//Sphere *S1 = new Sphere(dvec3(10.0, 1.0, -4.0), cyan, 1.0, MIRROR);

	//Tetrahedra
	Vertex Tetra1(5.0, 1, -2.0); Vertex Tetra2(8.0, -1.0, -2.0);
	Vertex Tetra3(8.0, 3.0, -2.0); Vertex Tetra4(6.5, 1, 2.0);
	vector<Vertex> Tetrahedron1 = { Tetra1, Tetra2, Tetra4 }; sceneTri.push_back(Triangle(Tetrahedron1, yellow, LAMBERTIAN));
	vector<Vertex> Tetrahedron2 = { Tetra1, Tetra4, Tetra3 }; sceneTri.push_back(Triangle(Tetrahedron2, yellow, LAMBERTIAN));
	vector<Vertex> Tetrahedron3 = { Tetra3, Tetra4, Tetra2 }; sceneTri.push_back(Triangle(Tetrahedron3, yellow, LAMBERTIAN));
	vector<Vertex> Tetrahedron4 = { Tetra1, Tetra3, Tetra2 }; sceneTri.push_back(Triangle(Tetrahedron4, yellow, LAMBERTIAN));

	//Vertices
	Vertex aBot(-3.0, 0.0, -5.0); Vertex aTop(-3.0, 0.0, 5.0);
	Vertex bBot(0.0, 6.0, -5.0); Vertex bTop(0.0, 6.0, 5.0);
	Vertex cBot(10.0, 6.0, -5.0); Vertex cTop(10.0, 6.0, 5.0);
	Vertex dBot(13.0, 0.0, -5.0); Vertex dTop(13.0, 0.0, 5.0);
	Vertex eBot(10.0, -6.0, -5.0); Vertex eTop(10.0, -6.0, 5.0);
	Vertex fBot(0.0, -6.0, -5.0); Vertex fTop(0.0, -6.0, 5.0);

	//Floor
	vector<Vertex> t1 = { aBot, fBot, bBot }; sceneTri.push_back(Triangle(t1, green, LAMBERTIAN));
	vector<Vertex> t2 = { bBot, fBot, cBot }; sceneTri.push_back(Triangle(t2, green, LAMBERTIAN));
	vector<Vertex> t3 = { fBot, eBot, cBot }; sceneTri.push_back(Triangle(t3, green, LAMBERTIAN));
	vector<Vertex> t4 = { dBot, cBot, eBot }; sceneTri.push_back(Triangle(t4, green, LAMBERTIAN));

	//Roof
	vector<Vertex> t5 = { aTop, bTop, fTop }; sceneTri.push_back(Triangle(t5, green, LAMBERTIAN));
	vector<Vertex> t6 = { bTop, cTop, fTop }; sceneTri.push_back(Triangle(t6, green, LAMBERTIAN));
	vector<Vertex> t7 = { fTop, cTop, eTop }; sceneTri.push_back(Triangle(t7, green, LAMBERTIAN));
	vector<Vertex> t8 = { dTop, eTop, cTop }; sceneTri.push_back(Triangle(t8, green, LAMBERTIAN));

	//Right backside
	vector<Vertex> t9 = { fBot, aTop, fTop }; sceneTri.push_back(Triangle(t9, red, LAMBERTIAN));
	vector<Vertex> t10 = { aTop, fBot, aBot }; sceneTri.push_back(Triangle(t10, red, LAMBERTIAN));

	//Left backside
	vector<Vertex> t11 = { bTop, aTop, aBot }; sceneTri.push_back(Triangle(t11, blue, LAMBERTIAN));
	vector<Vertex> t12 = { bTop, aBot, bBot }; sceneTri.push_back(Triangle(t12, blue, LAMBERTIAN));

	//Right frontside
	vector<Vertex> t13 = { eTop, dTop, eBot }; sceneTri.push_back(Triangle(t13, red, LAMBERTIAN));
	vector<Vertex> t14 = { eBot, dTop, dBot }; sceneTri.push_back(Triangle(t14, red, LAMBERTIAN));

	//Left frontside
	vector<Vertex> t15 = { cTop, cBot, dTop }; sceneTri.push_back(Triangle(t15, blue, LAMBERTIAN));
	vector<Vertex> t16 = { cBot, dBot, dTop }; sceneTri.push_back(Triangle(t16, blue, LAMBERTIAN));

	//Right center
	vector<Vertex> t17 = { fBot, eTop, eBot }; sceneTri.push_back(Triangle(t17, white, LAMBERTIAN));
	vector<Vertex> t18 = { fTop, eTop, fBot }; sceneTri.push_back(Triangle(t18, white, LAMBERTIAN));

	//Left center
	vector<Vertex> t19 = { cTop, bTop, bBot }; sceneTri.push_back(Triangle(t19, white, LAMBERTIAN));
	vector<Vertex> t20 = { bBot, cBot, cTop }; sceneTri.push_back(Triangle(t20, white, LAMBERTIAN));

	//Test
	Scene sc(sceneTri); sc.sphere = S1;
	Camera cam(2);

	cout << "Rendering.." << endl;
	cam.render(sc);
	cout << "Finished!" << endl;

	cout << "Creating Image" << endl;
	cam.createImage();
	cout << "Finished!" << endl;

	cout << "Done!";

	return 0;
}