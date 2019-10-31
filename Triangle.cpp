#include "Triangle.h"
#include "Camera.h"
#include "OBJ_Loader.h"

Triangle::Triangle()
:surface(DIFFUSE)
{
	vertices.push_back(Vertex(0.0, 0.0, 0.0));
	vertices.push_back(Vertex(0.0, 0.0, 0.0));
	vertices.push_back(Vertex(0.0, 0.0, 0.0));
	colorDbl = dvec3(1.0, 1.0, 1.0);
	normal = vec3(0.0, 0.0, 0.0);

}


Triangle::Triangle(vector<Vertex> &in_vertices, dvec3 in_colorDbl, Surface in_surface)
:vertices(in_vertices), colorDbl(in_colorDbl), surface(in_surface)
{
	vec3 v0 = vertices[0].pos;
	vec3 v1 = vertices[1].pos;
	vec3 v2 = vertices[2].pos;

	normal = normalize(cross(v1 - v0,v2 - v0));
}

//möller-trumbore algorithm
bool Triangle::rayIntersection(Ray &arg)
{
	vec3 v0 = vertices[0].pos;
	vec3 v1 = vertices[1].pos;
	vec3 v2 = vertices[2].pos;

	vec3 T = vec3(arg.start.pos) - v0; //T
	vec3 E1 = v1 - v0; //E1
	vec3 E2 = v2 - v0; // E2
	vec3 D = arg.dir; //D
	vec3 P = cross(D,E2); //P
	vec3 Q = cross(T, E1); 
	float a = dot(E1, P);

	//Backface culling
	if (a < EPS) return false;
	//Ray is parallel to triangle
	if (a > -EPS && a < EPS) return false;

	float t, u, v;
	u = dot(P, T) / a; if (u < 0.0f || u > 1.0f) return false;
	v = dot(Q, D) / a; if (v < 0.0f|| u + v > 1.0f || v > 1.0f) return false;
	t = dot(Q, E2) / a;

	//Ray intersection
	if(t > EPS)
	{
		arg.end.pos = vec4(vec3(arg.start.pos) + arg.dir * t, 1.0f);
		return true;
	}

	return false;
}

Triangle& Triangle::operator=(Triangle lhs)
{
	swap(vertices, lhs.vertices);
	swap(colorDbl, lhs.colorDbl);
	swap(normal, lhs.normal);
	swap(surface, lhs.surface);

	return *this;
}

ostream& operator<<(ostream &out, Triangle t)
{
	out << "Triangle: \n";
	
	for(int i = 0; i < 3; ++i)
	{
		out << "\tVertex " << i << ": " << t.vertices[i];
	}

	out << "\t" << "Normal: ";  print(t.normal); out << endl;
	out << "\t" << "Color: "; print(t.colorDbl); out << endl;
	return out;
}

void loadMeshToScene(vector<Triangle> &triangles, string filename, dvec3 in_color, Surface in_surface)
{
	objl::Loader Loader;

	bool loadout = Loader.LoadFile(filename);

	if(loadout)
	{
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		for(int i = 0; i < curMesh.Indices.size(); i += 3)
		{
			Vertex v1 = Vertex(curMesh.Vertices[curMesh.Indices[i]].Position.X + 4.0f,
				               curMesh.Vertices[curMesh.Indices[i]].Position.Y,
				               curMesh.Vertices[curMesh.Indices[i]].Position.Z - 3.0f);
			Vertex v2 = Vertex(curMesh.Vertices[curMesh.Indices[i + 1]].Position.X + 4.0f,
				               curMesh.Vertices[curMesh.Indices[i + 1]].Position.Y,
				               curMesh.Vertices[curMesh.Indices[i + 1]].Position.Z - 3.0f);
			Vertex v3 = Vertex(curMesh.Vertices[curMesh.Indices[i + 2]].Position.X + 4.0f,
				               curMesh.Vertices[curMesh.Indices[i + 2]].Position.Y,
				               curMesh.Vertices[curMesh.Indices[i + 2]].Position.Z - 3.0f);
			vector<Vertex> tri = {v1, v2, v3};
			float n_x = (curMesh.Vertices[curMesh.Indices[i]].Normal.X,
				         curMesh.Vertices[curMesh.Indices[i + 1]].Normal.X,
				         curMesh.Vertices[curMesh.Indices[i + 2]].Normal.X) / 3.0f;
			float n_y = (curMesh.Vertices[curMesh.Indices[i]].Normal.Y,
						 curMesh.Vertices[curMesh.Indices[i + 1]].Normal.Y,
						 curMesh.Vertices[curMesh.Indices[i + 2]].Normal.Y) / 3.0f;
			float n_z = (curMesh.Vertices[curMesh.Indices[i]].Normal.Z,
						 curMesh.Vertices[curMesh.Indices[i + 1]].Normal.Z,
						 curMesh.Vertices[curMesh.Indices[i + 2]].Normal.Z) / 3.0f;
			Triangle t = Triangle(tri, in_color, in_surface);
			t.normal = normalize(vec3(n_x, n_y, n_z));
			triangles.emplace_back(t);
		}
		cout << "MODEL LOADED: " << filename << endl;
	}
	else cout << "COULD NOT LOAD MODEL: " << filename << endl;
}