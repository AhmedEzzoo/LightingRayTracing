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

void Renderer::Render(const std::vector<Sphere>& spheres, const Camera& camera)
{
	Ray ray;
	ray.RayOrigin = camera.GetPosition();


	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			ray.RayDirection = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];
		
			glm::vec4 color = TraceRay(spheres, ray);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = CovertToRGBA(color);

		}

	}


	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::TraceRay(const std::vector <Sphere>& spheres, const Ray& ray)
{
	// (bx^2 + by^2)t^2 + 2(ax * bx  +  ay * by)t  + (ax^2 + ay^2 - r^2) = 0
	if (spheres.size() == 0)
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);


	const Sphere* closestSphere = nullptr;
	float hitPosition = std::numeric_limits<float>::max();


	for (const Sphere& sphere : spheres)
	{

		glm::vec3 origin = ray.RayOrigin - sphere.Center;

		float a = glm::dot(ray.RayDirection, ray.RayDirection);
		float b = 2.0f * glm::dot(origin, ray.RayDirection);
		float c = glm::dot(origin, origin) - sphere.Raduis * sphere.Raduis;

		// b^2 - 4ac
		// (-b + sqrt(discriminant)) / 2 * a
		// (-b - sqrt(discriminant)) / 2 * a
		float discriminant = b * b - 4.0f * a * c;
		if (discriminant < 0.0f)
			continue;

		float closest = (-b - glm::sqrt(discriminant)) / (2.0f * a);

		if (closest < hitPosition)
		{
			hitPosition = closest;
			closestSphere = &sphere;
		}
	}

	if (closestSphere == nullptr)
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);


	glm::vec3 origin = ray.RayOrigin - closestSphere->Center;

	glm::vec3 hitPoint = origin + ray.RayDirection * hitPosition;
	glm::vec3 normal = glm::normalize(hitPoint);

	glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));
	float lightIntenisty = glm::max(glm::dot(normal, -lightDir), 0.0f);


	glm::vec3 sphereColor = closestSphere->Color;
	sphereColor *= lightIntenisty;

	return glm::vec4(sphereColor, 1.0f);
}
