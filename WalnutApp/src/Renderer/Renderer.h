#pragma once

#include "glm/glm.hpp"
#include "Walnut/Image.h"

#include <memory>
#include "Camera.h"
#include "Ray.h"

#include "Scene.h"

class Renderer
{
public :
	Renderer() = default;
	~Renderer();
	
	const std::shared_ptr<Walnut::Image>& GetImage() const { return m_FinalImage; }
	
	void OnResize(uint32_t width, uint32_t height);

	void Render(const Scene& scene, const Camera& camera);

	void ResetFrameIndex() { m_FrameIndex = 1; }


	struct Settings
	{
		bool Accumulate = true;
	};

	Settings& GetSettings() { return m_Settings; }

private :

	struct HitPayload
	{
		float HItDistance;
		glm::vec3 WorldPosition;
		glm::vec3 WorldNormal;


		int ObjectIndex;
	};



	glm::vec4 PerPixel(uint32_t x, uint32_t y);
	
	HitPayload TraceRay(const Ray& ray);
	HitPayload Miss(const Ray& ray);
	HitPayload ClosestHit(const Ray& ray, HitPayload payload);



private :

	const Scene* m_ActiveScene = nullptr;
	const Camera* m_Camera = nullptr;

	std::shared_ptr<Walnut::Image> m_FinalImage;

	uint32_t m_Width = 0;
	uint32_t m_Height = 0;

	Settings m_Settings;

	uint32_t* m_ImageData = nullptr;
	glm::vec4* m_AccumulatedData = nullptr;

	uint32_t m_FrameIndex = 1;
};