#pragma once

#include <glm/glm.hpp>

namespace Snow
{

	class Camera
	{
	public:
		Camera(glm::mat4 projection)
			: m_Projection(projection) {};

		const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
	private:
		glm::mat4 m_Projection;
	};

};