#pragma once

#include "Core/Renderer/Framebuffer.h"

namespace Snow
{

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& fbspec);
		virtual ~OpenGLFramebuffer() override;

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(const uint32_t width, const uint32_t height) override;

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specs; }
		virtual uint32_t GetColorAttachementRendererID(int index = 0) const override { return m_ColorAttachments[index]; }
	private:
		uint32_t m_RendererID = 0;

		FramebufferSpecification m_Specs;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
		FramebufferTextureSpecification m_DepthAttachmentSpec;

		std::vector<uint32_t> m_ColorAttachments;

		uint32_t m_DepthAttachment = 0;
	};
}