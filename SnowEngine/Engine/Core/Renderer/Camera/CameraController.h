#pragma once

#include "Camera.h"
#include "SnowEngineAPI.h"
#include "../../Events/MouseEvent.h"
#include "../../Events/ApplicationEvent.h"
#include "../../Timestep.h"
namespace Snow
{

	class CameraController
	{
	public:
		virtual ~CameraController() = default;
		CameraController() {};

		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnEvent(Event& e) = 0;

		virtual void OnResize(float width, float height) = 0;

		virtual Camera& GetCamera() = 0;

	private:
		virtual bool OnMouseScrolledEvent(MouseScrolledEvent& e) = 0;
		virtual bool OnWindowResizeEvent(WindowResizeEvent& e) = 0;
	};
};