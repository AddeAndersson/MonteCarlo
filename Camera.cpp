#include "Camera.h"
#include "Scene.h"
#include "Triangle.h"
#include <glm/gtx/vector_angle.hpp>
#include "Sphere.h"
#include "Vertex.h"


#define _USE_MATH_DEFINES
#include <cmath>

dvec3 findIntersection(Scene &sc, Ray &r, Pixel &p, int depth);
random_device dev;
mt19937 generator(dev());
int shadowRayCount = 1;

Camera::Camera(int in_chosenEye)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		vector<Pixel> temp;

		for (int j = 0; j < WIDTH; j++)
		{
			temp.emplace_back(dvec3(0.0, 0.0, 0.0));
		}

		pixels.push_back(temp);
		temp.clear();
	}
	chosenEye = in_chosenEye;
}

void Camera::createImage()
{
	FILE *fp = fopen("rendering4.ppm", "wb");
	(void)fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
	float avg = 0.0f;

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			static unsigned char clr[3];
			dvec3 color = pixels[i][j].colorDbl;
			clr[0] = (color.x * 255.99) / maxVal; avg += color.x;
			clr[1] = (color.y * 255.99) / maxVal; avg += color.y;
			clr[2] = (color.z * 255.99) / maxVal; avg += color.z;
			
			(void)fwrite(clr, 1, 3, fp);
		}
	}
	(void)fclose(fp);
	avg /= (HEIGHT * WIDTH * 3.0f * maxVal);
	cout << "Max: " << maxVal << endl;
	cout << "Average: " << avg << endl;
}

void Camera::render(Scene &sc)
{
	Vertex eye = (chosenEye == 1) ? eye1 : eye2;
	vec3 launchColor(1, 1, 1);
	const float dPix = 2.0f / HEIGHT;
	const float offset = 1.0f - dPix / 2.0f;

	auto start = chrono::steady_clock::now();

	//for(int i = 0; i < HEIGHT; i++)
	Concurrency::parallel_for(int(0), HEIGHT, [&](int i) //Multi-Threading for performance boost
	{
		for (int j = 0; j < WIDTH; j++)
		{
			//Ray randomization
			uniform_real_distribution<float> pixelJitter(0.0f, dPix / samples);

			//Find intersection in scene
			for (int n = 0; n < samples; n++)
			{
				for (int m = 0; m < samples; m++)
				{
					float jitterW = pixelJitter(generator);
					float jitterH = pixelJitter(generator);

					//Launch ray from eye towards the pixels in the camera
					auto pixW = offset - j * dPix + (dPix / 2.0f) + jitterW;
					auto pixH = offset - i * dPix + (dPix / 2.0f) + jitterH;

					dvec3 dir = dvec3(0, pixW - m * dPix / samples, pixH - n * dPix / samples) - dvec3(eye.pos);
					Ray r = Ray(eye, dir, launchColor);
					//Find intersection in scene
					pixels[i][j].colorDbl += findIntersection(sc, r, pixels[i][j], 0);
				}
			}

			pixels[i][j].colorDbl /= samples * samples;

			//Update max and min
			maxVal = std::max(maxVal, (pixels[i][j].colorDbl.x + pixels[i][j].colorDbl.y + pixels[i][j].colorDbl.z) / 3.0);
		}

		if (i % (HEIGHT / 20) == 0) cout << setw(4) << round(i * 100 / HEIGHT) << "%" << endl;
		}); //parallel loop
	    //};  //for loop

	auto end = chrono::steady_clock::now();

	cout << "Rendering finished in: "
		 << chrono::duration_cast<chrono::seconds>(end - start).count()
		 << " Seconds." << endl;
}

ostream& operator<<(ostream& out, Camera c)
{
	Vertex v = (c.chosenEye == 1) ? c.eye1 : c.eye2;
	out << "Camera: " << endl;
	out << "\tEye: " << v;

	return out;
}

dvec3 findIntersection(Scene &sc, Ray &r, Pixel &p, int depth)
{
	float currDist = 999.0f;
	bool useSphere = false;
	Triangle closestTri;
	Sphere closestSp;
	vec3 sphereNormal;

	//find closest intersecting triangle
	for (auto &t : sc.triangles)
	{
		if (t.rayIntersection(r))
		{
			currDist = length(vec3(r.end.pos - r.start.pos));
			closestTri = t;
			break;
		}
	}

	//test intersection with sphere
	for(auto &s : sc.spheres)
	{
		if (rayIntersectionSphere(r, s))
		{
			if (length(vec3(r.end.pos - r.start.pos)) < currDist)
			{
				useSphere = true;
				sphereNormal = normalize(vec3(r.end.pos) - s.center);
				closestSp = s;
				break;
			}
		}
	}

	uniform_real_distribution<float> uniform(0.0f, 1.0f);

	//Is sphere closer?
	if(useSphere)
	{
		switch (closestSp.surface.type)
		{
			case DIFFUSE:
			{	
				float rr = uniform(generator);

				if(rr < 0.75f && depth < 10) //ray is partly reflected
				{
					p.ray = &r;

					float r1 = uniform(generator);
					float r2 = uniform(generator);

					float phi_out = r1 * 2.0f * PI; //random azimuth angle for outgoing ray [0, 2*PI]
					float theta_out = asin(sqrt(r2)); //"random" inclination, optimized for near normal's direction

					//sample local hemisphere
					vec3 sample(sin(theta_out) * cos(phi_out), sin(theta_out) * sin(phi_out), cos(theta_out));

					//transform to world coordinates
					vec3 Nt;
					if (std::fabs(sphereNormal.x) > std::fabs(sphereNormal.z))
					Nt = normalize(vec3(-sphereNormal.y, sphereNormal.x, 0.0f));
					else Nt = normalize(vec3(0.0f, -sphereNormal.z, sphereNormal.y));

					vec3 Nb = normalize(cross(sphereNormal, Nt));
					vec3 reflectedDir(sample.z * sphereNormal.x + sample.y * Nb.x + sample.x * Nt.x,
		                              sample.z * sphereNormal.y + sample.y * Nb.y + sample.x * Nt.y,
		                              sample.z * sphereNormal.z + sample.y * Nb.z + sample.x * Nt.z);

					Ray reflectedRay = Ray(r.end, reflectedDir, r.color * closestSp.colorDbl);

					//Some portion of the ray is absorbed and some is reflected in a random direction
					return closestSp.surface.absorption * r.color * closestSp.colorDbl * static_cast<double>(closestSp.surface.resolveDiffuse(sc, r, shadowRayCount, sphereNormal))
						   + (1.0 - closestSp.surface.absorption) * findIntersection(sc, reflectedRay, p, depth + 1);

				}
				else //completely absorbed
				{
					p.ray = &r;
					return r.color * closestSp.colorDbl * static_cast<double>(closestSp.surface.resolveDiffuse(sc, r, shadowRayCount, sphereNormal));
				}
				
			}
			case MIRROR:
			{
				if (depth < 10)
				{
					//ray is perfectly reflected in a new direction
					//vec3 reflectedDir = r.dir - 2.0f * dot(r.dir, sphereNormal) * sphereNormal;
					vec3 reflectedDir = reflect(r.dir, sphereNormal);
					Ray reflectedRay = Ray(r.end, reflectedDir, r.color);
					//return findIntersection(sc, reflectedRay, p, depth + 1);
					return closestSp.surface.absorption * r.color * closestSp.colorDbl * static_cast<double>(closestSp.surface.resolveDiffuse(sc, r, shadowRayCount, sphereNormal))
						+ (1.0f - closestSp.surface.absorption) * findIntersection(sc, reflectedRay, p, depth + 1);
				}
				else return dvec3(0.0, 0.0, 0.0);
			}
			case TRANSPARENT:
			{
				if (depth < 10)
				{
					float n1 = 1.00027717f; //refractive index of air
					float n2 = 1.5168f; //refractive index of glass
					float NdotI = clamp(-1.0f, 1.0f, dot(r.dir, sphereNormal));

					//entering object
					if (NdotI < 0.0f) NdotI = -NdotI;
					else//leaving object
					{
						sphereNormal = -sphereNormal;
						swap(n1, n2);
					}

					float c1 = NdotI;
					float n = n1 / n2;
					float c2 = 1.0f - (n * n) * (1.0f - (c1 * c1));

					//vec3 refractedDir = n * r.dir + sphereNormal * (-n * c1 - sqrt(c2));
					vec3 refractedDir = refract(r.dir, sphereNormal, n);
					Ray refractedRay = Ray(r.end, refractedDir, r.color);

					//vec3 reflectedDir = r.dir - 2.0f * NdotI * sphereNormal;
					vec3 reflectedDir = reflect(r.dir, sphereNormal);
					Ray reflectedRay = Ray(r.end, reflectedDir, r.color);

					//total internal reflection
					if (c2 < EPS) return findIntersection(sc, reflectedRay, p, depth + 1);

					//reflection and transmission
					float R_0 = pow((n1 - n2) / (n1 + n2), 2.0f);
					float R_theta = R_0 + (1.0f - R_0) * pow(1.0f - NdotI, 5.0f);
					float T_theta = 1.0f - R_theta;

					return static_cast<double>(R_theta) * findIntersection(sc, reflectedRay, p, depth + 1)
						+ static_cast<double>(T_theta) * findIntersection(sc, refractedRay, p, depth + 1);
				}
				return vec3(0.0f, 0.0f, 0.0f);
			
			}
			case LIGHT:
			{
				return dvec3(1.0, 1.0, 1.0);
			}
		}
	}

	//Intersection is with a triangle
	switch (closestTri.surface.type)
	{
		case DIFFUSE:
		{	
			float rr = uniform(generator);

			if (rr < 0.75f && depth < 10) //reflect portion of ray
			{
				p.ray = &r;

				float r1 = uniform(generator);
				float r2 = uniform(generator);

				float phi_out = r1 * 2.0f * PI; //random azimuth angle for outgoing ray [0, 2*PI]
				float theta_out = asin(sqrt(r2)); //"random" inclination, optimized for near normal's direction

				//sample local hemisphere
				vec3 sample(sin(theta_out) * cos(phi_out), sin(theta_out)*sin(phi_out), cos(theta_out));

				//transform to world coordinates
				vec3 Nt;
				if (std::fabs(closestTri.normal.x) > std::fabs(closestTri.normal.z))
				Nt = normalize(vec3(-closestTri.normal.y, closestTri.normal.x, 0.0f));
				else Nt = normalize(vec3(0.0f, -closestTri.normal.z, closestTri.normal.y));

				vec3 Nb = normalize(cross(closestTri.normal, Nt));
				vec3 reflectedDir(sample.z * closestTri.normal.x + sample.y * Nb.x + sample.x * Nt.x,
		                          sample.z * closestTri.normal.y + sample.y * Nb.y + sample.x * Nt.y,
		                          sample.z * closestTri.normal.z + sample.y * Nb.z + sample.x * Nt.z);

				Ray reflectedRay = Ray(r.end, reflectedDir, r.color * closestTri.colorDbl);

				//some portion of the ray is absorbed and some is reflected in a random direction
				return closestTri.surface.absorption * r.color * closestTri.colorDbl * static_cast<double>(closestTri.surface.resolveDiffuse(sc, r, shadowRayCount, closestTri.normal))
					   + (1.0 - closestTri.surface.absorption) * findIntersection(sc, reflectedRay, p, depth + 1);

			}
			else //ray is completely absorbed
			{
				p.ray = &r;
				return r.color * closestTri.colorDbl * static_cast<double>(closestTri.surface.resolveDiffuse(sc, r, shadowRayCount, closestTri.normal));
			}
		}
		case MIRROR:
		{
			if(depth < 10)
			{
				//ray is perfectly reflected in a new direction
				//vec3 reflectedDir = r.dir - 2.0f * dot(r.dir, closestTri.normal) * closestTri.normal;
				vec3 reflectedDir = reflect(r.dir, closestTri.normal);
				Ray reflectedRay = Ray(r.end, reflectedDir, r.color);
				//return findIntersection(sc, reflectedRay, p, depth);
				return closestTri.surface.absorption * r.color * closestTri.colorDbl * static_cast<double>(closestTri.surface.resolveDiffuse(sc, r, shadowRayCount, closestTri.normal))
				+ (1.0 - closestTri.surface.absorption) * findIntersection(sc, reflectedRay, p, depth + 1);
			}
			else return dvec3(0.0, 0.0, 0.0);
		}
		case TRANSPARENT: //handles both cases of ray entering/leaving thicker medium
		{
			if (depth < 10)
			{
				float n1 = 1.00027717f; //refractive index of air
				float n2 = 1.5168f; //refractive index of glass
				float NdotI = clamp(-1.0f, 1.0f, dot(r.dir, closestTri.normal));

				//entering object
				if (NdotI < 0.0f) NdotI = -NdotI;
				else//leaving object
				{
					closestTri.normal = -closestTri.normal;
					swap(n1, n2);
				}

				float c1 = NdotI;
				float n = n1 / n2;
				float c2 = 1.0f - (n * n) * (1.0f - (c1 * c1));

				//vec3 refractedDir = n * r.dir + closestTri.normal * (-n * c1 - sqrt(c2));
				vec3 refractedDir = refract(r.dir, closestTri.normal, n);
				Ray refractedRay = Ray(r.end, refractedDir, r.color);

				//vec3 reflectedDir = r.dir - 2.0f * NdotI * closestTri.normal;
				vec3 reflectedDir = reflect(r.dir, closestTri.normal);
				Ray reflectedRay = Ray(r.end, reflectedDir, r.color);

				//total internal reflection
				if (c2 < EPS) return findIntersection(sc, reflectedRay, p, depth + 1);

				//reflection and transmission
				float R_0 = pow((n1 - n2) / (n1 + n2), 2.0f);
				float R_theta = R_0 + (1.0f - R_0) * pow(1.0f - NdotI, 5.0f);
				float T_theta = 1.0f - R_theta;

				return static_cast<double>(R_theta) * findIntersection(sc, reflectedRay, p, depth + 1)
					+ static_cast<double>(T_theta) * findIntersection(sc, refractedRay, p, depth + 1);
			}
			return vec3(0.0f, 0.0f, 0.0f);
		}
		case LIGHT:
		{
			return dvec3(1.0, 1.0, 1.0);
		}

	}
}

