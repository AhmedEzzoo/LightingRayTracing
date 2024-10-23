#pragma once

#include "glm/glm.hpp"

struct Sphere
{

	glm::vec3 Center{0.0f, 0.0f, 0.0f};
	float Raduis = 0.5f;
	glm::vec3 Color{1.0f, 0.0f, 1.0f};

	Sphere(const glm::vec3& color, float raduis, const glm::vec3& center)
		: Color(color), Raduis(raduis), Center(center)
	{}
	
};


struct Scene
{

	std::vector<Sphere> Spheres;

};
