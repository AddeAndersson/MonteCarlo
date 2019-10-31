#pragma once
#include <bitmap/bitmap_image.hpp>
#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <ppl.h>
#include "Pixel.h"


class Scene;
const int HEIGHT = 400;
const int WIDTH = 400;
const int samples = 4; //root of total number of samples per pixel
const float PI = 3.141592653f;
const float EPS = 1e-3f;

using namespace std;

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
		double maxVal = -9999.0;

		vector<vector<Pixel>> pixels;
};

