#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Renderer/Renderer.h"

#include "Renderer/Camera.h"
#include "Renderer/Scene.h"

#include "Walnut/Timer.h"

#include "glm/gtc/type_ptr.hpp"

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer() : m_Camera(45.0f, 0.1f, 100.0f)
	{
		{
			Material mat;
			mat.Color = { 1.0f, 0.0f, 1.0f };
			mat.Roughness = 0.0f;
			mat.Metallic = 0.0f;


			Sphere pinkSphere;
			pinkSphere.Center = { 0.0f, 0.0f, 0.0f };
			pinkSphere.Raduis = 1.0f;
			pinkSphere.MaterialIndex = 0;
			m_Scene.Spheres.emplace_back(pinkSphere);
			m_Scene.Mat.emplace_back(mat);
		}

		{
			Material mat;
			mat.Color = { 0.0f, 0.0f, 1.0f };
			mat.Roughness = 0.0f;
			mat.Metallic = 0.0f;

			Sphere blueSphere;
			blueSphere.Center = { 0.0f, -101.0f, 0.0f };
			blueSphere.Raduis = 100.00f;
			blueSphere.MaterialIndex = 1;
			m_Scene.Spheres.emplace_back(blueSphere);
			m_Scene.Mat.emplace_back(mat);

		}
		

	}

	virtual void OnUpdate(float ts) override 
	{
		if (m_Camera.OnUpdate(ts))
			m_Renderer.ResetFrameIndex();
	}

	virtual void OnUIRender() override
	{
		
		
		ImGui::Begin("Settings");
		ImGui::Text("Renderer Time %0.2f", m_RenderTime);

		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::End();

		ImGui::Begin("Controls");

		for (uint32_t i = 0; i < m_Scene.Spheres.size(); i++)
		{
			ImGui::PushID(i);

			ImGui::DragFloat3("Position", glm::value_ptr(m_Scene.Spheres[i].Center), 0.1f);
			ImGui::DragFloat("Raduis", &(m_Scene.Spheres[i].Raduis), 0.1f);
			ImGui::DragInt("Index", &(m_Scene.Spheres[i].MaterialIndex), 1, 0, (int)m_Scene.Mat.size() - 1);

			ImGui::Separator();

			ImGui::PopID();

		}

		for (uint32_t i = 0; i < m_Scene.Mat.size(); i++)
		{
			ImGui::PushID(i);

			ImGui::ColorEdit3("Color", glm::value_ptr(m_Scene.Mat[i].Color));
			ImGui::DragFloat("Roughness", &(m_Scene.Mat[i].Roughness), 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat("Metallic", &(m_Scene.Mat[i].Metallic), 0.05f, 0.0f, 1.0f);

			ImGui::Separator();

			ImGui::PopID();

		}

		ImGui::Checkbox("Accumulate", &(m_Renderer.GetSettings().Accumulate));

		if (ImGui::Button("ResetFrameIndex"))
		{
			m_Renderer.ResetFrameIndex();
		}
		


		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
		ImGui::Begin("ViewPort");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		

		if (m_Renderer.GetImage())
			ImGui::Image(m_Renderer.GetImage()->GetDescriptorSet(), ImVec2{ (float)m_Renderer.GetImage()->GetWidth(), (float)m_Renderer.GetImage()->GetHeight()}, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });


		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	
	}

	void Render()
	{
		Walnut::Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_Scene, m_Camera);

		m_RenderTime = timer.ElapsedMillis();
	}


private :
	Renderer m_Renderer;
	Camera m_Camera;
	Scene m_Scene;

	uint32_t m_ViewportWidth = 0;
	uint32_t m_ViewportHeight = 0;

	float m_RenderTime = 0.0f;

};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}