#pragma once

#include "Core/Renderer/Framebuffer.h"

namespace Snow
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specs);
		virtual ~OpenGLFramebuffer() override;

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(const uint32_t width, const uint32_t height) override;

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specs; }
		virtual uint32_t GetColorAttachementRendererID() const override { return m_ColorAttachment; }
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
		FramebufferSpecification m_Specs;
	};
}