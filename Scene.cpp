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
	vector<Sphere> sceneSp;

	//Colors
	dvec3 white(1.0, 1.0, 1.0);
	dvec3 red(1.0, 0.0, 0.0);
	dvec3 yellow(1.0, 1.0, 0.2);
	dvec3 cyan(0.2, 1.0, 1.0);
	dvec3 green(0.0, 1.0, 0.0);
	dvec3 blue(0.0, 0.0, 1.0);

	//Sphere
	Sphere S1(vec3(4.0f, 0.5f, 1.0f), cyan, 1.0f, Surface(TRANSPARENT));
	Sphere S2(dvec3(10.0f, -3.0f, 2.5f), cyan, 2.0f, MIRROR);
	sceneSp.push_back(S1); sceneSp.push_back(S2);

	//OBJ model
	//loadMeshToScene(sceneTri, "models/teapot.obj", red, Surface(MIRROR, 0.0f, 0.7));

	//Tetrahedra
	//    x ^    A---B
	//      |     \D/
	// y <--o      C
	/*Vertex TetraA(10.0f, -1.0f, -5.0f); Vertex TetraB(10.0f, -4.0f, -5.0f);
	Vertex TetraC(7.0f, -2.5f, -5.0f); Vertex TetraD(8.5f, -2.5f, -2.0f);
	vector<Vertex> Tetrahedron1 = { TetraB, TetraD, TetraC }; sceneTri.push_back(Triangle(Tetrahedron1, yellow, Surface(MIRROR)));
	vector<Vertex> Tetrahedron2 = { TetraA, TetraD, TetraB }; sceneTri.push_back(Triangle(Tetrahedron2, yellow, Surface(MIRROR)));
	vector<Vertex> Tetrahedron3 = { TetraC, TetraD, TetraA }; sceneTri.push_back(Triangle(Tetrahedron3, yellow, Surface(MIRROR)));
	vector<Vertex> Tetrahedron4 = { TetraA, TetraB, TetraC }; sceneTri.push_back(Triangle(Tetrahedron4, yellow, Surface(MIRROR)));*/

	//Cube
	//    x ^     B---C
	//      |     |   |
	// y <--o     A---D
	Surface cubeSurface(DIFFUSE, 0.5f, 0.5);
	Vertex CubeABot(7.0f, 4.0f, -5.0f); Vertex CubeATop(7.0f, 4.0f, -2.0f);
	Vertex CubeBBot(10.0f, 4.0f, -5.0f); Vertex CubeBTop(10.0f, 4.0f, -2.0f);
	Vertex CubeCBot(10.0f, 1.0f, -5.0f); Vertex CubeCTop(10.0f, 1.0f, -2.0f);
	Vertex CubeDBot(7.0f, 1.0f, -5.0f); Vertex CubeDTop(7.0f, 1.0f, -2.0f);
	vector<Vertex> Cube1 = { CubeABot, CubeDTop, CubeATop }; vector<Vertex> Cube7 = { CubeCBot, CubeCTop, CubeDTop };
	vector<Vertex> Cube2 = { CubeABot, CubeDBot, CubeDTop }; vector<Vertex> Cube8 = { CubeDTop, CubeDBot, CubeCBot };
	vector<Vertex> Cube3 = { CubeBBot, CubeATop, CubeBTop }; vector<Vertex> Cube9 = { CubeABot, CubeBBot, CubeCBot };
	vector<Vertex> Cube4 = { CubeBBot, CubeABot, CubeATop }; vector<Vertex> Cube10 = { CubeABot, CubeCBot, CubeDBot };
	vector<Vertex> Cube5 = { CubeBBot, CubeBTop, CubeCTop }; vector<Vertex> Cube11 = { CubeATop, CubeDTop, CubeCTop };
	vector<Vertex> Cube6 = { CubeCBot, CubeBBot, CubeCTop }; vector<Vertex> Cube12 = { CubeATop, CubeCTop, CubeBTop };
	sceneTri.emplace_back(Triangle(Cube1, white, cubeSurface)); sceneTri.emplace_back(Triangle(Cube7, white, cubeSurface));
	sceneTri.emplace_back(Triangle(Cube2, white, cubeSurface)); sceneTri.emplace_back(Triangle(Cube8, white, cubeSurface));
	sceneTri.emplace_back(Triangle(Cube3, white, cubeSurface)); sceneTri.emplace_back(Triangle(Cube9, white, cubeSurface));
	sceneTri.emplace_back(Triangle(Cube4, white, cubeSurface)); sceneTri.emplace_back(Triangle(Cube10, white, cubeSurface));
	sceneTri.emplace_back(Triangle(Cube5, white, cubeSurface)); sceneTri.emplace_back(Triangle(Cube11, white, cubeSurface));
	sceneTri.emplace_back(Triangle(Cube6, white, cubeSurface)); sceneTri.emplace_back(Triangle(Cube12, white, cubeSurface));

	//Area light source
	Vertex light1(6.0f, -1.0f, 5.0f); Vertex light2(6.0f, 1.0f, 5.0f); // 4--2 As seen from above
	Vertex light3(4.0f, -1.0f, 5.0f); Vertex light4(4.0f, 1.0f, 5.0f); // 3--1
	vector<Vertex> t21 = { light4, light2, light1 }; sceneTri.emplace_back(Triangle(t21, white, LIGHT));
	vector<Vertex> t22 = { light3, light4, light1 }; sceneTri.emplace_back(Triangle(t22, white, LIGHT));

	//Vertices for room layout
	Vertex aBot(-3.0f, 0.0f, -5.0f); Vertex aTop(-3.0f, 0.0f, 5.0f);
	Vertex bBot(0.0f, 6.0f, -5.0f); Vertex bTop(0.0f, 6.0f, 5.0f);
	Vertex cBot(10.0f, 6.0f, -5.0f); Vertex cTop(10.0f, 6.0f, 5.0f);
	Vertex dBot(13.0f, 0.0f, -5.0f); Vertex dTop(13.0f, 0.0f, 5.0f);
	Vertex eBot(10.0f, -6.0f, -5.0f); Vertex eTop(10.0f, -6.0f, 5.0f);
	Vertex fBot(0.0f, -6.0f, -5.0f); Vertex fTop(0.0f, -6.0f, 5.0f);

	//Floor
	Surface floor(DIFFUSE, 0.0f, 0.3);
	vector<Vertex> t1 = { aBot, fBot, bBot }; sceneTri.emplace_back(Triangle(t1, white, floor));
	vector<Vertex> t2 = { bBot, fBot, cBot }; sceneTri.emplace_back(Triangle(t2, white, floor));
	vector<Vertex> t3 = { fBot, eBot, cBot }; sceneTri.emplace_back(Triangle(t3, white, floor));
	vector<Vertex> t4 = { dBot, cBot, eBot }; sceneTri.emplace_back(Triangle(t4, white, floor));

	//Roof
	Surface roof(DIFFUSE, 0.0f, 0.3);
	vector<Vertex> t5 = { aTop, bTop, fTop }; sceneTri.emplace_back(Triangle(t5, white, roof));
	vector<Vertex> t6 = { bTop, cTop, fTop }; sceneTri.emplace_back(Triangle(t6, white, roof));
	vector<Vertex> t7 = { fTop, cTop, eTop }; sceneTri.emplace_back(Triangle(t7, white, roof));
	vector<Vertex> t8 = { dTop, eTop, cTop }; sceneTri.emplace_back(Triangle(t8, white, roof));

	//Right backside
	Surface walls(DIFFUSE, 0.0f, 0.4);
	vector<Vertex> t9 = { fBot, aTop, fTop }; sceneTri.emplace_back(Triangle(t9, red, walls));
	vector<Vertex> t10 = { aTop, fBot, aBot }; sceneTri.emplace_back(Triangle(t10, red, walls));

	//Left backside
	vector<Vertex> t11 = { bTop, aTop, aBot }; sceneTri.emplace_back(Triangle(t11, green, walls));
	vector<Vertex> t12 = { bTop, aBot, bBot }; sceneTri.emplace_back(Triangle(t12, green, walls));

	//Right frontside
	vector<Vertex> t13 = { eTop, dTop, eBot }; sceneTri.emplace_back(Triangle(t13, red, walls));
	vector<Vertex> t14 = { eBot, dTop, dBot }; sceneTri.emplace_back(Triangle(t14, red, walls));

	//Left frontside
	vector<Vertex> t15 = { cTop, cBot, dTop }; sceneTri.emplace_back(Triangle(t15, green, walls));
	vector<Vertex> t16 = { cBot, dBot, dTop }; sceneTri.emplace_back(Triangle(t16, green, walls));

	//Right center
	vector<Vertex> t17 = { fBot, eTop, eBot }; sceneTri.emplace_back(Triangle(t17, red, walls));
	vector<Vertex> t18 = { fTop, eTop, fBot }; sceneTri.emplace_back(Triangle(t18, red, walls));

	//Left center
	vector<Vertex> t19 = { cTop, bTop, bBot }; sceneTri.emplace_back(Triangle(t19, green, walls));
	vector<Vertex> t20 = { bBot, cBot, cTop }; sceneTri.emplace_back(Triangle(t20, green, walls));

	//Test
	Scene sc(sceneTri); sc.spheres = sceneSp;
	Camera cam(2);

	cout << "Rendering.." << endl;
	cam.render(sc);

	cout << "Creating Image" << endl;
	cam.createImage();
	cout << "Finished!" << endl;

	cout << "Done!" << endl;

	return 0;
}