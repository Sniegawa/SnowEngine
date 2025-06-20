#pragma once

namespace Snow
{
	enum class RendererAPI
	{
		None = 0,
		OpenGL,
		//Vulcan,
		//Direct3D
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; }

	private:
		static RendererAPI s_RendererAPI;
	};
};