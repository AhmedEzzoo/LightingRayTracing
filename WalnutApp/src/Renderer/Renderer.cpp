#include "Renderer.h"
#include "Walnut/Random.h"


Renderer::~Renderer()
{

}

static uint32_t CovertToRGBA(const glm::vec4& data)
{


	uint8_t r = (uint8_t)(data.r * 255.0f);
	uint8_t g = (uint8_t)(data.g * 255.0f);
	uint8_t b = (uint8_t)(data.b * 255.0f);
	uint8_t a = (uint8_t)(data.a * 255.0f);

	uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;

	return result;

}

void Renderer::OnResize(uint32_t width, uint32_t height)
{

	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_FinalImage->Resize(width, height);
	}
	else
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);


	delete[]m_ImageData;
	m_ImageData = new uint32_t[width * height];
}


void Renderer::Render(const Scene& scene, const Camera& camera)
{
	
	m_ActiveScene = &scene;
	m_Camera = &camera;

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
		
			glm::vec4 color = PerPixel(x, y);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = CovertToRGBA(color);

		}

	}


	m_FinalImage->SetData(m_ImageData);
}

Renderer::HitPayload Renderer::ClosestHit(const Ray& ray, HitPayload payload)
{
	const Sphere& closestSphere = m_ActiveScene->Spheres[payload.ObjectIndex];

	glm::vec3 origin = ray.RayOrigin - closestSphere.Center;
	payload.WorldPosition = origin + ray.RayDirection * payload.HItDistance;
	payload.WorldNormal = glm::normalize(payload.WorldPosition);

	payload.WorldPosition += closestSphere.Center;

	return payload;
}

glm::vec4 Renderer::PerPixel(uint32_t x, uint32_t y)
{
	Ray ray;
	ray.RayOrigin = m_Camera->GetPosition();
	ray.RayDirection = m_Camera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

	glm::vec3 color(0.0f);
	float multiplyer = 1.0f;


	uint8_t bounce = 2;
	for (uint8_t i = 0; i < bounce; i++)
	{
		HitPayload payload = TraceRay(ray);
		if (payload.HItDistance < 0.0f)
		{
			glm::vec3 skyColor  =  glm::vec3(0.0f, 0.0f, 0.0f);
			color += skyColor * multiplyer;
			break;
		}

		glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));
		float lightIntenisty = glm::max(glm::dot(payload.WorldNormal, -lightDir), 0.0f);


		const Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];

		glm::vec3 sphereColor = sphere.Color;
		sphereColor *= lightIntenisty;

		color += sphereColor * multiplyer;
		
		multiplyer *= 0.7f;
		ray.RayOrigin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
		ray.RayDirection = glm::reflect(ray.RayDirection, payload.WorldNormal);
	}

	

	return glm::vec4(color, 1.0f);
}

Renderer::HitPayload Renderer::TraceRay(const Ray& ray)
{
	// (bx^2 + by^2)t^2 + 2(ax * bx  +  ay * by)t  + (ax^2 + ay^2 - r^2) = 0
	


	int  closestSphere = -1;
	float hitDistance = std::numeric_limits<float>::max();


	for (size_t i = 0; i <  m_ActiveScene->Spheres.size(); i++)
	{

		glm::vec3 origin = ray.RayOrigin - m_ActiveScene->Spheres[i].Center;

		float a = glm::dot(ray.RayDirection, ray.RayDirection);
		float b = 2.0f * glm::dot(origin, ray.RayDirection);
		float c = glm::dot(origin, origin) - m_ActiveScene->Spheres[i].Raduis * m_ActiveScene->Spheres[i].Raduis;

		// b^2 - 4ac
		// (-b + sqrt(discriminant)) / 2 * a
		// (-b - sqrt(discriminant)) / 2 * a
		float discriminant = b * b - 4.0f * a * c;
		if (discriminant < 0.0f)
			continue;

		float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);

		if (closestT > 0.0f && closestT < hitDistance)
		{
			hitDistance = closestT;
			closestSphere = (int)i;
		}
	}

	if (closestSphere < 0)
		return Miss(ray);
	
	HitPayload payload;
	payload.HItDistance = hitDistance;
	payload.ObjectIndex = closestSphere;

	return ClosestHit(ray, payload);
	
}

Renderer::HitPayload Renderer::Miss(const Ray& ray)
{
	HitPayload payload;
	payload.HItDistance = -1;

	return payload;
}
