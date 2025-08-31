#pragma once

#include "Core/Timestep.h"
#include "Core/Events/Event.h"
#include "Core/Events/MouseEvent.h"
#include "Core/Events/ApplicationEvent.h"

#include "OrthographicCamera.h"

namespace Snow
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(glm::vec3 pos, float rotation, float aspectRatio, bool allowRotation);
		~OrthographicCameraController();
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return m_Camera; }

		inline float GetZoomLevel() const { return m_ZoomLevel; }
		inline void SetZoomLevel(float level) { m_ZoomLevel = level; }

	private:
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	
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