#pragma once

#include <SnowEngine/SnowEngine.h>

class Sandbox2D : public Snow::Layer
{
public:
	Sandbox2D();
	~Sandbox2D();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Snow::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Snow::Event& e) override;

private:
	glm::vec3 m_SquarePosition = glm::vec3(0.0f,0.0f,1.0f);
	float rotation = 0.0f;
	glm::vec2 scale = glm::vec2(1.0f);
	glm::vec3 tint = glm::vec3(1.0f);
	float m_speed = 2.0f;

	Snow::ShaderLibrary m_ShaderLib;
	Snow::OrthographicCameraController m_CameraController;

	Snow::Ref<Snow::VertexArray> m_SquareVA;

	Snow::Ref<Snow::Texture2D> m_Texture;
	Snow::Ref<Snow::Texture2D> m_Texture2;


};
