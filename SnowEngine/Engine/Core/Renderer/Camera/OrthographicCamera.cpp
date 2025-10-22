#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Snow
{
	OrthographicCamera::OrthographicCamera(glm::vec3 position, float rotation, CameraParams::OrthographicCameraParams params)
		: m_Position(position), m_Rotation(rotation)
	{
		RecalculateProjectionMatrix(params);
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		RecalculateViewMatrix();
	}

	const glm::vec3& OrthographicCamera::GetPosition() const
	{
		return m_Position;
	}

	void OrthographicCamera::SetRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation; // Note : for orthographic camera we only rotate in z axis
		RecalculateViewMatrix();
	}

	const glm::vec3& OrthographicCamera::GetRotation() const
	{
		return m_Rotation;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = 
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0, 0, 1)) *
			glm::translate(glm::mat4(1.0f), m_Position);


		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateProjectionMatrix(CameraParams::OrthographicCameraParams params)
	{
		m_ProjectionMatrix = glm::ortho(params.left, params.right, params.bottom, params.top,-10.0f,10.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

};
