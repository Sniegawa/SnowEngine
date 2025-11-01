#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Snow
{

	namespace Utils
	{
		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8:
			case FramebufferTextureFormat::RED_INTEGER:
				return false;
			case FramebufferTextureFormat::None:
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				return true;
			default:
				SNOW_CORE_ERROR("Unknwon Framebuffer Texture Format");
				return false;
				break;
			}
		}

		static std::pair<GLenum,GLenum> FormatType(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case Snow::FramebufferTextureFormat::None:
				SNOW_CORE_ERROR("Can't retrieve Format and type from None texture format");
				return { 0,0 };
				break;
			case Snow::FramebufferTextureFormat::RGBA8:
				return { GL_RGBA,GL_UNSIGNED_BYTE };
				break;
			case Snow::FramebufferTextureFormat::RED_INTEGER:
				return { GL_RED_INTEGER,GL_INT };
				break;
			case Snow::FramebufferTextureFormat::DEPTH24STENCIL8:
				return { GL_DEPTH_STENCIL_ATTACHMENT , GL_UNSIGNED_INT_24_8 };
				break;
			default:
				SNOW_CORE_ERROR("Can't retrieve Format and type from None texture format");
				return { 0,0 };
				break;
			}
		}

		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalformat, FramebufferTextureFormat format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalformat, width, height, GL_FALSE);
			}
			else
			{
				auto [dataformat, datatype] = Utils::FormatType(format);
				glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, dataformat, datatype, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum internalformat, GLenum attachmentType ,uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalformat, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, internalformat, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& fbspec)
		: m_Specs(fbspec)
	{
		for (auto spec : m_Specs.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
				m_ColorAttachmentSpecs.emplace_back(spec);
			else
			{
				if (m_DepthAttachmentSpec.TextureFormat != FramebufferTextureFormat::None)
					SNOW_CORE_WARN("Snow engine doesnt support multiple depth textures on one framebuffer");
				m_DepthAttachmentSpec = spec;
			}
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1,& m_DepthAttachment);

		m_ColorAttachments.clear();
		m_DepthAttachment = 0;
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);
			glDeleteFramebuffers(1, &m_RendererID);
			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}
		
		if (m_Specs.Width == 0 || m_Specs.Height == 0)
			return;


		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);


		bool multisample = m_Specs.Samples > 1;
		//Attachments
		if (!m_ColorAttachmentSpecs.empty())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecs.size());
			Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachmentSpecs.size(); ++i)
			{
				Utils::BindTexture(multisample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecs[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specs.Samples, GL_RGBA8, m_ColorAttachmentSpecs[i].TextureFormat, m_Specs.Width, m_Specs.Height, i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specs.Samples, GL_R32I, m_ColorAttachmentSpecs[i].TextureFormat, m_Specs.Width, m_Specs.Height, i);
					break;
				default:
					break;
				}
			}
		}

		if (m_DepthAttachmentSpec.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
			Utils::BindTexture(multisample, m_DepthAttachment);
			switch (m_DepthAttachmentSpec.TextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specs.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specs.Width, m_Specs.Height);
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			SNOW_CORE_ASSERT(m_ColorAttachments.size() <= 4, "NO MORE THAN 4");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2 ,GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((GLsizei)m_ColorAttachments.size(), buffers);
		}

		else if (m_ColorAttachments.empty())
		{
			//Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		SNOW_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(const uint32_t width, const uint32_t height)
	{
		m_Specs.Width = width;
		m_Specs.Height = height;
		this->Invalidate();
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		Bind();

		SNOW_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(),"");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;

		auto& texformat = m_ColorAttachmentSpecs[attachmentIndex].TextureFormat;
		auto [format, datatype] = Utils::FormatType(texformat);

		glReadPixels(x, y, 1, 1,format, datatype,&pixelData); // TODO make not hard-coded
		Unbind();

		return pixelData;
	}

	//TODO: think about templating the function and not use type from Utils function
	//Because the data format should rely on internal format not just a format and that's not in the function
	void OpenGLFramebuffer::ClearAttachment(int idx, const void* value)
	{
		SNOW_CORE_ASSERT(idx < m_ColorAttachments.size(), "idx for clear should be in range of color attachments");

		auto& texformat = m_ColorAttachmentSpecs[idx].TextureFormat;
		bool multisample = m_Specs.Samples > 1;

		auto [format, datatype] = Utils::FormatType(texformat);

		glClearTexImage(m_ColorAttachments[idx], 0, format, datatype, value);
	}


	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specs.Width, m_Specs.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};