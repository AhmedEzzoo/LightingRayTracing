#pragma once

#include <vector>

#include "glm/glm.hpp"


class Camera
{
public:
	Camera(float fov, float nearClip, float farClip);
	~Camera() = default;

	const glm::mat4& GetProjection() const { return m_Projection; }
	void SetProjection(const glm::mat4& projection) { m_Projection = projection; }
	const glm::mat4& GetInverseProjection() const { return m_InversProjection; }

	const glm::mat4& GetView() const { return m_View; }
	const glm::mat4& GetInverseView() const { return m_InverseView; }
	
	const glm::vec3& GetPosition() const { return m_Position; }
	const glm::vec3& GetDirection() const { return m_ForwardDirection; }

	const std::vector<glm::vec3>& GetRayDirections() const { return m_RayDirections; }



	const glm::mat4& GetViewProjection() const { return m_Projection * m_View; }


	void OnResize(uint32_t width, uint32_t height);

	bool OnUpdate(float ts);

private :
	void RecalculateProjection();
	void RecalculateView();
	void RecalculateRayDirections();

private :
	glm::mat4 m_Projection = { 1.0f };
	glm::mat4 m_View = { 1.0f };
	glm::mat4 m_InversProjection = { 1.0f };
	glm::mat4 m_InverseView = { 1.0f };
	float m_FOV = 45.0f;

	uint32_t m_ViewportWidth = 0;
	uint32_t m_ViewportHeight = 0;


	float m_NearClip = 0.1f;
	float m_FarClip = 100.0f;

	glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_ForwardDirection{ 0.0f, 0.0f, 0.0f };

	glm::vec2  m_LastMousePosition = { 0.0f, 0.0f };
	std::vector<glm::vec3> m_RayDirections;

};