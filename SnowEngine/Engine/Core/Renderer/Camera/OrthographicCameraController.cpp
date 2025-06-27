#include "OrthographicCameraController.h"

#include "Core/Input.h"

namespace Snow
{
	OrthographicCameraController::OrthographicCameraController(glm::vec3 pos, float rotation, float aspectRatio,bool allowRotation)
		: m_AspectRatio(aspectRatio),m_CameraPosition(pos),m_CameraRotation(rotation), m_Camera(pos, rotation, {-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel}), m_AllowRotation(allowRotation)
	{
		m_Camera.RecalculateProjectionMatrix({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel });
	}

	OrthographicCameraController::~OrthographicCameraController()
	{

	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;

		CameraParams::OrthographicCameraParams params(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		m_Camera.RecalculateProjectionMatrix(params);
	}

	Camera& OrthographicCameraController::GetCamera()
	{
		return m_Camera; 
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		float dt = ts;

		if (Snow::Input::IsKeyPressed(Snow::Key::A))
		{
			m_CameraPosition += (glm::vec3(-1.0f, 0.0f, 0.0f) * m_CameraTranslationSpeed * dt);
		}
		if (Snow::Input::IsKeyPressed(Snow::Key::D))
		{
			m_CameraPosition += (glm::vec3(1.0f, 0.0f, 0.0f) * m_CameraTranslationSpeed * dt);
		}
		if (Snow::Input::IsKeyPressed(Snow::Key::W))
		{
			m_CameraPosition +=(glm::vec3(0.0f, 1.0f, 0.0f) * m_CameraTranslationSpeed * dt);
		}
		if (Snow::Input::IsKeyPressed(Snow::Key::S))
		{
			m_CameraPosition += (glm::vec3(0.0f, -1.0f, 0.0f) * m_CameraTranslationSpeed * dt);
		}


		if (m_AllowRotation)
		{
			if (Input::IsKeyPressed(Key::Q))
				m_CameraRotation += m_CameraRotationSpeed * dt;
			if (Input::IsKeyPressed(Key::E))
				m_CameraRotation -= m_CameraRotationSpeed * dt;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(glm::vec3(0.0f,0.0f,m_CameraRotation)); // This looks bad, might change something in camera class later to prevent
		}

		m_Camera.SetPosition(m_CameraPosition);

		
		m_CameraTranslationSpeed = m_ZoomLevel;

	}
	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(SNOW_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolledEvent, 1));
		dispatcher.Dispatch<WindowResizeEvent>(SNOW_BIND_EVENT_FN(OrthographicCameraController::OnWindowResizeEvent, 1));
	}
	bool OrthographicCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		CameraParams::OrthographicCameraParams params(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		m_Camera.RecalculateProjectionMatrix(params);

		return false;
	}
	bool OrthographicCameraController::OnWindowResizeEvent(WindowResizeEvent & e)
	{
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
};