#include "Surface.h"
#include "Camera.h"
#include "Triangle.h"
#include "Sphere.h"


Surface::Surface(int in_type)
:type(in_type), roughness(0.5f)
{
	if (in_type == MIRROR) absorption = 0.0f;
}

Surface::Surface(int in_type, float in_roughness, double in_absorption)
:type(in_type), roughness(in_roughness), absorption(in_absorption)
{
}


float Surface::resolveDiffuse(Scene &sc, Ray &r, int numberOfShadowRays, vec3 in_normal)
{
	random_device dev;
	mt19937 genShadow(dev());
	uniform_real_distribution<float> distUniformX(4.0f, 6.0f); //4.0 - 6.0 Light source position in x
	uniform_real_distribution<float> distUniformY(-1.0f, 1.0f); //Light source position in y
	float sum = 0.0f;
	float L0 = 1000.0f / (PI * 4.0f);

	for (int i = 0; i < numberOfShadowRays; ++i) //number of shadow rays
	{
		bool intersected = false;
		
		float BRDF;
		float toX = distUniformX(genShadow);
		float toY = distUniformY(genShadow);

		//sends a shadow ray to a random point on the area light source
		vec3 shadowRayDir = normalize(vec3(toX, toY, 5.0f) - vec3(r.end.pos));
		vec3 startPos = vec3(r.end.pos);
		Vertex shadowRayStartPos = Vertex(startPos.x, startPos.y, startPos.z);
		Ray shadowRay = Ray(shadowRayStartPos, shadowRayDir, dvec3(0.0, 0.0, 0.0));

		float distLight = length(vec3(toX, toY, 5.0f) - vec3(r.end.pos));
		float in_angle = acos(dot(shadowRayDir, in_normal));

		//OREN-NAYAR SURFACE
		if (roughness > EPS)
		{
			normal_distribution<float> distNormal(0.0f, roughness);

			float phi_out = distNormal(genShadow) * PI; //random azimuth angle for outgoing ray
			float theta_out = asin(sqrt(distNormal(genShadow))); //"random" inclination, optimized for near normal's direction

			float A = 1.0f - 0.5f * (roughness * roughness) / (roughness * roughness + 0.33f);
			float B = 0.45f * (roughness * roughness) / (roughness * roughness + 0.09f);

			float alpha = std::max(in_angle, theta_out); //max of in_angle and theta_out
			float beta = std::min(in_angle, theta_out); //min of in_angle and theta_out
			float maxPhi = std::max(cos(-phi_out), 0.0f); //max of cos(-phi_out) and 0

			BRDF = A + (B * maxPhi) * sin(alpha) * tan(beta);
		}
		//LAMBERTIAN SURFACE
		//might add albedo/pi to return
		else BRDF = 1.0f;
		
		
		float alpha_k = clamp(dot(-shadowRayDir, vec3(0.0f, 0.0f, -1.0f)), 0.0f, 1.0f);
		float beta_k = clamp(dot(shadowRayDir, in_normal), 0.0f, 1.0f);
		float contribution = alpha_k * BRDF * beta_k / (distLight * distLight);

		//intersection with sphere
		for(auto &s : sc.spheres)
		if (rayIntersectionSphere(shadowRay, s) && (s.surface.type == DIFFUSE || s.surface.type == MIRROR)) 
			intersected = true;

		//intersection with triangle
		for (auto &t2 : sc.triangles)
		{
			if (t2.rayIntersection(shadowRay))
			{
				//distance to closest triangle in the direction from the intersection point
				//to make sure we dont cast shadows from the ceiling
				float distTri = length(vec3(shadowRay.end.pos) - vec3(shadowRay.start.pos));

				//if distance to a triangle is closer than the distance to the light, the intersection point is occluded
				//transparent surfaces are ignored, furthermore we compare distTri to a small distance to avoid surface acne
				if(distTri > EPS && distTri < distLight && (t2.surface.type == DIFFUSE || t2.surface.type == MIRROR))
				{
					//intersection point is occluded
					intersected = true;
					break;
				}
			}
		}

		if (!intersected)
		{
			sum += contribution;
		}

	}
	return  0.9f * 4.0f * L0 * sum / (static_cast<float>(numberOfShadowRays) * 3.0f);
}