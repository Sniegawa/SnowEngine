#pragma once
#include "CameraController.h"
#include "OrthographicCamera.h"

namespace Snow
{
	class OrthographicCameraController : CameraController
	{
	public:
		OrthographicCameraController(glm::vec3 pos, float rotation, float aspectRatio, bool allowRotation);
		~OrthographicCameraController();
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& e) override;

		virtual void OnResize(float width, float height) override;

		virtual Camera& GetCamera() override { return m_Camera; }

		inline float GetZoomLevel() const { return m_ZoomLevel; }
		inline void SetZoomLevel(float level) { m_ZoomLevel = level; }

	private:
		virtual bool OnMouseScrolledEvent(MouseScrolledEvent& e) override;
		virtual bool OnWindowResizeEvent(WindowResizeEvent& e) override;
	
	private:
		OrthographicCamera m_Camera;

		bool m_AllowRotation;

		float m_AspectRatio = 1.7f;
		float m_ZoomLevel = 1.0f;
		
		glm::vec3 m_CameraPosition;
		float m_CameraRotation;
		
		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 5.0f;
	};
};