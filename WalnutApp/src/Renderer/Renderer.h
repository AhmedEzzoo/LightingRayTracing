#pragma once

#include "glm/glm.hpp"
#include "Walnut/Image.h"

#include <memory>
#include "Camera.h"
#include "Ray.h"

#include "Sphere.h"

class Renderer
{
public :
	Renderer() = default;
	~Renderer();
	
	const std::shared_ptr<Walnut::Image>& GetImage() const { return m_FinalImage; }
	
	void OnResize(uint32_t width, uint32_t height);

	void Render(const std::vector<Sphere>& spheres, const Camera& camera);



private :
	glm::vec4 TraceRay(const std::vector <Sphere>& spheres, const Ray& ray);

private :

	uint32_t* m_ImageData = nullptr;
	std::shared_ptr<Walnut::Image> m_FinalImage;

	uint32_t m_Width = 0;
	uint32_t m_Height = 0;

};