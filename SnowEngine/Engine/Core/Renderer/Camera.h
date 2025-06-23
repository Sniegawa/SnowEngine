#pragma once

#include <glm/glm.hpp>

namespace Snow
{

	class Camera
	{
	public:
		Camera() : m_ProjectionMatrix(0.0f), m_ViewMatrix(0.0f), m_ViewProjectionMatrix(0.0f) {};
		virtual ~Camera() {};
		
		virtual void Move(const glm::vec3& movement) = 0;
		virtual void Rotate(const glm::vec3& rotation) = 0;

		virtual void SetPosition(const glm::vec3& position) = 0;
		virtual const glm::vec3& GetPosition() const = 0;

		virtual void SetRotation(const glm::vec3& rotation) = 0;
		virtual const glm::vec3& GetRotation() const = 0;

		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		virtual void RecalculateMatrices() = 0;

	protected:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};

};