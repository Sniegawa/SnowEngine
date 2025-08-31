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

	//Todo : remove
	//Deprecated code
	class OrthographicCamera
	{
	public:
		OrthographicCamera(glm::vec3 position = glm::vec3(0.0f), float rotation = 0.0f, CameraParams::OrthographicCameraParams params = CameraParams::OrthographicCameraParams());
		~OrthographicCamera() {};

		void SetPosition(const glm::vec3& position);
		const glm::vec3& GetPosition() const;

		void SetRotation(const glm::vec3& rotation);
		const glm::vec3& GetRotation() const;

		void RecalculateProjectionMatrix(CameraParams::OrthographicCameraParams params);

		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();
	private:
		glm::vec3 m_Position;
		float m_Rotation;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};
};