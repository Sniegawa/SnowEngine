#pragma once
#include "Camera.h"

namespace Snow
{
	namespace CameraParams
	{
		struct OrthographicCameraParams
		{
			float left;
			float right;
			float bottom;
			float top;

			OrthographicCameraParams(float _left = -1.6f, float _right = 1.6f, float _bottom = -0.9f, float _top = 0.9f) :
				left(_left), right(_right), bottom(_bottom), top(_top)
			{}
		};
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(glm::vec3 position = glm::vec3(0.0f), float rotation = 0.0f, CameraParams::OrthographicCameraParams params = CameraParams::OrthographicCameraParams());
		~OrthographicCamera() {};

		virtual void SetPosition(const glm::vec3& position) override;
		virtual const glm::vec3& GetPosition() const override;

		virtual void SetRotation(const glm::vec3& rotation) override;
		virtual const glm::vec3& GetRotation() const override;

		void RecalculateProjectionMatrix(CameraParams::OrthographicCameraParams params);

	private:
		virtual void RecalculateViewMatrix() override;
	private:
		glm::vec3 m_Position;
		float m_Rotation;
	};
};