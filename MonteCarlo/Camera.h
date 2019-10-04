#pragma once
#include <bitmap/bitmap_image.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "Pixel.h"

class Scene;
const int HEIGHT = 400;
const int WIDTH = 400;
const double PI = 3.1415926535898;
const double EPS = 1e-4;

class Camera
{
public:
	Camera(int in_chosenEye);
	~Camera() = default;
	void render(Scene &sc);
	void createImage();
	friend ostream& operator<<(ostream &out, Camera c);

	Vertex eye1 = Vertex(-2, 0, 0);
	Vertex eye2 = Vertex(-1, 0, 0);
	int chosenEye;
	double max = -9999.0;
	double min = 9999.0;

	vector<vector<Pixel>> pixels;
};

