#pragma once

#include <basicIncludes.h>
#include <Texture.h>
#include <Shader.h>
#include <QuadRenderer.h>

class Framebuffer
{
public:
	struct RenderBuffer
	{
		GLenum attachment = GL_COLOR_ATTACHMENT0;
		GLenum format = GL_RGBA16F;

		RenderBuffer(GLenum attachment = GL_COLOR_ATTACHMENT0)
			: attachment(attachment)
		{
			if (attachment == GL_DEPTH_ATTACHMENT)
			{
				format = GL_DEPTH_COMPONENT32F;
			}
		}
		RenderBuffer(GLenum attachment, GLenum format)
			: attachment(attachment), format(format) {
		}

		operator GLenum()
		{
			return attachment;
		}
		operator int()
		{
			return attachment;
		}
	};

private:
	uint32_t FBO = 0;
	uint32_t FBOFX = 0;
	uint32_t* renderBuffers = nullptr;
	uint32_t* renderBuffersFX = nullptr;
	int renderBufferCount = 0;
	std::vector<std::pair<std::string, RenderBuffer>> renderBuffersV;
	std::vector<GLenum> colorBuffers;
	uint32_t msaaSamples = 4;
	int w = 1, h = 1;

public:
	Shader* shader = nullptr;

	Framebuffer() { }
	Framebuffer(GLsizei width, GLsizei height, const std::vector<std::pair<std::string, RenderBuffer>>& renderBuffers, uint32_t msaaSamples = 4);
	void init(GLsizei width, GLsizei height, const std::vector<std::pair<std::string, RenderBuffer>>& renderBuffers = {}, uint32_t msaaSamples = -1);
	~Framebuffer();

	void render(bool doBlitFX = true);
	void cleanup();
	void use() const { glBindFramebuffer(GL_FRAMEBUFFER, FBO); }
	inline static void useDefault() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
	void blitFX();

	uint32_t id() const { return FBO; }
	uint32_t idFX() const { return FBOFX; }
	uint32_t rboID(int i = 0) const { if (i >= 0 && i < renderBufferCount && renderBuffers) return renderBuffers[i]; return 0; }
	uint32_t rboIDFX(int i = 0) const { if (i >= 0 && i < renderBufferCount && renderBuffersFX) return renderBuffersFX[i]; return 0; }

	template<size_t _Size = 0>
	void drawColorBuffers(const std::array<GLenum, _Size>& colorBuffers) const
	{
		if constexpr (_Size == 0)
		{
			return glNamedFramebufferDrawBuffers(FBO, this->colorBuffers.size(), this->colorBuffers.data());
		}
		return glNamedFramebufferDrawBuffers(FBO, _Size, colorBuffers.data());
	}
	void drawColorBuffers() const
	{
		return glNamedFramebufferDrawBuffers(FBO, this->colorBuffers.size(), this->colorBuffers.data());
	}
};
