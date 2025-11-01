#pragma once

#include <SnowEngineAPI.h>

namespace Snow
{

	enum class FramebufferTextureFormat
	{
		None = 0,

		//Color
		RGBA8,
		RED_INTEGER,

		//Depth/Stencil
		DEPTH24STENCIL8,

		//Default
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format){ }

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		//Filtering & wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}
		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t Width,Height;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		//Render to screen
		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual const FramebufferSpecification& GetSpecification() const = 0;
		virtual uint32_t GetColorAttachementRendererID(int index = 0) const = 0;
		
		virtual void ClearAttachment(int idx,const void* value) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(const uint32_t width, const uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& specs);
	};
};