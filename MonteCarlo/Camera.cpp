#include "Camera.h"

const int HEIGHT = 400;
const int WIDTH = 400;

Camera::Camera(int in_chosenEye)
{
	

	for(int i = 0; i < HEIGHT; ++i)
	{
		std::vector<Pixel> temp;

		for(int j = 0; j < WIDTH; ++j)
		{
			temp.push_back(Pixel(glm::vec3(1.0, 1.0, 1.0)));
		}

		pixels.push_back(temp);
	}
	chosenEye = in_chosenEye;
}

void Camera::createImage()
{
	
}

void Camera::render()
{
	
}

