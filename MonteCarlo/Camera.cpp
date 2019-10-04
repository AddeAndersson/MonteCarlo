#include "Camera.h"
#include "Scene.h"
#include "Triangle.h"
#include <glm/gtx/vector_angle.hpp>
#include "Sphere.h"

#define _USE_MATH_DEFINES
#include <cmath>

bool isOccluded(Scene &sc, Ray *r, dvec3 lightSource);
dvec3 findIntersection(Scene &sc, Ray *r, Pixel &p);

Camera::Camera(int in_chosenEye)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		vector<Pixel> temp;

		for (int j = 0; j < WIDTH; j++)
		{
			temp.push_back(Pixel(vec3(0, 0, 0)));
		}

		pixels.push_back(temp);
		temp.clear();
	}
	chosenEye = in_chosenEye;
}

void Camera::createImage()
{
	FILE *fp = fopen("rendering3.ppm", "wb");
	(void)fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			static unsigned char clr[3];
			dvec3 color = pixels[i][j].colorDbl;
			clr[0] = 255 * color.x / max;
			clr[1] = 255 * color.y / max;
			clr[2] = 255 * color.z / max;

			(void)fwrite(clr, 1, 3, fp);
		}
	}
	(void)fclose(fp);
}

void Camera::render(Scene &sc)
{
	Vertex eye = (chosenEye == 1) ? eye1 : eye2;
	//dvec3 lightSource = vec3(2.0, 0.0, 4.0);
	vec3 launchColor(1, 1, 1);
	const double dPix = 2.0 / HEIGHT;
	const double offset = 1.0 - dPix;

	for(int i = 0; i < HEIGHT; i++)
	{
		for(int j = 0; j < WIDTH; j++)
		{
			//Launch ray from eye towards the pixels in the camera
			auto pixW = offset - j * dPix;
			auto pixH = offset - i * dPix;
			double ang;
			dvec3 dir = dvec3(0, pixW, pixH) - dvec3(eye.pos);
			Ray *r = new Ray(eye, dir, launchColor);

			//Find intersection in scene
			pixels[i][j].colorDbl = findIntersection(sc, r, pixels[i][j]);

			//Update max and min
			if (max < pixels[i][j].colorDbl.x) max = pixels[i][j].colorDbl.x;
			if (max < pixels[i][j].colorDbl.y) max = pixels[i][j].colorDbl.y;
			if (max < pixels[i][j].colorDbl.z) max = pixels[i][j].colorDbl.z;

			if (min > pixels[i][j].colorDbl.x) min = pixels[i][j].colorDbl.x;
			if (min > pixels[i][j].colorDbl.y) min = pixels[i][j].colorDbl.y;
			if (min > pixels[i][j].colorDbl.z) min = pixels[i][j].colorDbl.z;
			
		}
	}
}

ostream& operator<<(ostream& out, Camera c)
{
	Vertex v = (c.chosenEye == 1) ? c.eye1 : c.eye2;
	out << "Camera: " << endl;
	out << "\tEye: " << v;

	return out;
}

bool isOccluded(Scene &sc, Ray *r, dvec3 lightSource)
{
	Ray *shadowRay = new Ray(Vertex(r->end.pos.x, r->end.pos.y, r->end.pos.z), (lightSource - dvec3(r->end.pos)), dvec3(0.0, 0.0, 0.0));
	double distLight = length(lightSource - dvec3(r->end.pos));

	//intersection with sphere
	if (rayIntersectionSphere(*shadowRay, *sc.sphere))
	{
		return (length(dvec3(shadowRay->start.pos) - sc.sphere->center) > sc.sphere->radius);
	}

	//intersection with triangle
	for (auto &t2 : sc.triangles)
	{
		if (t2.rayIntersection(*shadowRay))
		{
			//distance to closest triangle in the direction from the intersection point
			double distTri = length(dvec3(shadowRay->end.pos) - dvec3(shadowRay->start.pos));

			//if distance to a triangle is closer than the distance to the light, the intersection point is occluded
			if (distTri < distLight)
			{
				//intersection point is occluded
				return true;
			}	
		}
	}

	//no intersection
	return false;
}

dvec3 findIntersection(Scene &sc, Ray *r, Pixel &p)
{
	dvec3 lightSource = dvec3(2.0, 0.0, 4.0);
	double ang;
	double currDist = 999.0;
	bool useSphere = false;
	//vector<pair<Triangle, double>> distances;
	Triangle closestTri;
	dvec3 sphereNormal;

	//find closest intersecting triangle
	for (auto &t : sc.triangles)
	{
		if (t.rayIntersection(*r))
		{
			//**TODO
			/*if(length(dvec3(r->end.pos - r->start.pos)) < currDist)
			{
				currDist = length(dvec3(r->end.pos - r->start.pos));
				closestTri = t;

			}*/
			currDist = length(dvec3(r->end.pos - r->start.pos));
			closestTri = t;
			break;
			//distances.emplace_back(make_pair(t, length(dvec3(r->end.pos - r->start.pos))));
		}
	}

	//**TODO 
	/*if (distances.size() == 0) return dvec3(0.0, 0.0, 0.0);
	sort(distances.begin(), distances.end(),
		[](const pair<Triangle, double>& lhs, const pair<Triangle, double>& rhs) {
		return lhs.second < rhs.second; });

	closestTri = distances[0].first;*/

	//test intersection with sphere
	if (rayIntersectionSphere(*r, *sc.sphere))
	{
		if(length(dvec3(r->end.pos - r->start.pos)) < currDist)
		{
			useSphere = true;
			sphereNormal = normalize(dvec3(r->end.pos) - sc.sphere->center);
		}
	}

	//Is sphere closer?
	if(useSphere)
	{
		switch (sc.sphere->surface)
		{
			case LAMBERTIAN:
			{
				ang = acos(dot(normalize(lightSource - dvec3(r->end.pos)), sphereNormal));

				//intersection with triangle or sphere is in shadowed area
				if (isOccluded(sc, r, lightSource))
				{
					p.ray = r;
					return dvec3(0.0, 0.0, 0.0);
				}

				//light reaches this point
				if (abs(ang) < PI / 2)
				{
					p.ray = r;
					return sc.sphere->colorDbl*cos(ang);
				}
				//light does not reach this point
				p.ray = r;
				return dvec3(0.0, 0.0, 0.0);
			}
			case OREN:
			{
				//**TODO
				return dvec3(0.0, 0.0, 0.0);
			}
			case MIRROR:
			{
				//ray is perfectly reflected in a new direction
				dvec3 reflectedDir = r->dir - 2.0 * dot(r->dir, sphereNormal) * sphereNormal;
				Ray *reflectedRay = new Ray(r->end, reflectedDir, r->color);
				return findIntersection(sc, reflectedRay, p);
			}
			case TRANSPARENT:
			{
				//**TODO
				return dvec3(0.0, 0.0, 0.0);
			}
			
		}
	}

	//Intersection is with a triangle
	switch (closestTri.surface)
	{
		case LAMBERTIAN:
		{	
			ang = acos(dot(normalize(lightSource - dvec3(r->end.pos)), closestTri.normal));

			//intersection with triangle or sphere is in shadowed area
			if (isOccluded(sc, r, lightSource))
			{
				p.ray = r;
				return dvec3(0.0, 0.0, 0.0);
			}

			//light reaches this point
			if (abs(ang) < PI / 2)
			{
				p.ray = r;
				return closestTri.colorDbl*cos(ang);
			}
			//light does not reach this point
			p.ray = r;
			return dvec3(0.0, 0.0, 0.0);
		}
		case OREN:
		{
			//**TODO
			return dvec3(0.0, 0.0, 0.0);
		}
		case MIRROR:
		{
			//ray is perfectly reflected in a new direction
			dvec3 reflectedDir = r->dir - 2.0 * dot(r->dir, closestTri.normal) * closestTri.normal;
			Ray *reflectedRay = new Ray(r->end, reflectedDir, r->color);
			return findIntersection(sc, reflectedRay, p);
		}
		case TRANSPARENT:
		{
			//**TODO
			return dvec3(0.0, 0.0, 0.0);
		}

	}
}

