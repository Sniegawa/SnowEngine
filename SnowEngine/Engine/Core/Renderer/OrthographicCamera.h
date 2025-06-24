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

			OrthographicCameraParams(float _left = -1.0f, float _right = 1.0f, float _bottom = -1.0f, float _top = 1.0f) :
				left(_left), right(_right), bottom(_bottom), top(_top)
			{

			}
		};
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(glm::vec3 position = glm::vec3(0.0f), float rotation = 0.0f, CameraParams::OrthographicCameraParams params = CameraParams::OrthographicCameraParams());
		~OrthographicCamera() {};

		virtual void Move(const glm::vec3& movement) override;
		virtual void Rotate(const glm::vec3& rotation) override;

		virtual void SetPosition(const glm::vec3& position) override;
		virtual const glm::vec3& GetPosition() const override;

		virtual void SetRotation(const glm::vec3& rotation) override;
		virtual const glm::vec3& GetRotation() const override;

	private:
		virtual void RecalculateMatrices() override ;
	private:
		glm::vec3 m_Position;
		float m_Rotation;
	};
};