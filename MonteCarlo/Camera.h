#pragma once
#include <glm/glm.hpp>
#include "Vertex.h"
#include <bitmap/bitmap_image.hpp>
#include "Pixel.h"

class Camera
{
public:
	Camera(int in_chosenEye);
	~Camera() = default;
	void render();
	void createImage();

	Vertex eye1 = Vertex(-2, 0, 0);
	Vertex eye2 = Vertex(-1, 0, 0);
	int chosenEye;

	std::vector<std::vector<Pixel>> pixels;
};

