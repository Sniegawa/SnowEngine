#pragma once

#include <SnowEngineAPI.h>

namespace Snow
{

	struct FramebufferSpecification
	{
		uint32_t Width,Height;
		uint32_t Samples = 1;

		//Render to screen
		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual const FramebufferSpecification& GetSpecification() const = 0;
		virtual uint32_t GetColorAttachementRendererID() const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(const uint32_t width, const uint32_t height) = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& specs);
	};
};