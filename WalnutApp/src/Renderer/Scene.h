#pragma once

#include "glm/glm.hpp"



struct Material
{
	glm::vec3 Color{ 1.0f, 0.0f, 1.0f };
	float Roughness = 0.1f;
	float Metallic = 0.0f;
};



struct Sphere
{

	glm::vec3 Center{0.0f, 0.0f, 0.0f};
	float Raduis = 0.5f;
	

	int MaterialIndex = 0;
};


struct Scene
{
	std::vector<Sphere> Spheres;
	std::vector<Material> Mat;
};
