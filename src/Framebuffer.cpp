#include <Framebuffer.h>

Framebuffer::Framebuffer(GLsizei width, GLsizei height, const std::vector<std::pair<std::string, RenderBuffer>>& renderBuffers, uint32_t msaaSamples)
{
	init(width, height, renderBuffers, msaaSamples);
}

void Framebuffer::init(GLsizei width, GLsizei height, const std::vector<std::pair<std::string, RenderBuffer>>& renderBuffers, uint32_t msaaSamples)
{
	if (!renderBuffers.empty())
	{
		renderBufferCount = renderBuffers.size();
		this->renderBuffersV = renderBuffers;
	}
	else
	{
		assert(renderBufferCount != 0);
		if (w == width && h == height) return;
	}

	if (msaaSamples != -1)
		this->msaaSamples = msaaSamples;

	cleanup();

	w = width;
	h = height;

	this->renderBuffers = new uint32_t[renderBufferCount](0);
	this->renderBuffersFX = new uint32_t[renderBufferCount](0);

	glCreateFramebuffers(1, &FBO);
	glCreateFramebuffers(1, &FBOFX);

	glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, renderBufferCount, this->renderBuffers);
	glCreateTextures(GL_TEXTURE_2D, renderBufferCount, this->renderBuffersFX);

	{
		std::vector<GLenum> colorAttachments;
		int i = 0;
		for (auto& type : this->renderBuffersV)
		{
			uint32_t tex = this->renderBuffers[i];
			uint32_t texFX = this->renderBuffersFX[i];

			if (type.second.attachment != GL_DEPTH_ATTACHMENT)
			{
				colorAttachments.emplace_back(type.second.attachment);
			}

			glTextureStorage2DMultisample(tex, this->msaaSamples, type.second.format, w, h, false);
			glNamedFramebufferTexture(FBO, type.second.attachment, tex, 0);

			glTextureStorage2D(texFX, 1, type.second.format, w, h);
			glTextureParameteri(texFX, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTextureParameteri(texFX, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(texFX, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(texFX, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glNamedFramebufferTexture(FBOFX, type.second.attachment, texFX, 0);

			++i;
		}

		glNamedFramebufferDrawBuffers(FBO, colorAttachments.size(), colorAttachments.data());
		glNamedFramebufferDrawBuffers(FBOFX, colorAttachments.size(), colorAttachments.data());

		colorBuffers = colorAttachments;
	}
}

Framebuffer::~Framebuffer()
{
	cleanup();
}

void Framebuffer::render(bool doBlitFX)
{
	if (doBlitFX)
		blitFX();

	if (!shader) return;

	shader->use();

	int i = 0;
	for (auto& b : renderBuffersV)
	{
		shader->setUniform(b.first, i);
		glBindTextureUnit(i, renderBuffersFX[i]);
		++i;
	}

	QuadRenderer::render();
}

void Framebuffer::cleanup()
{
	if (FBO)
	{
		glDeleteFramebuffers(1, &FBO);
		FBO = 0;

		glDeleteTextures(renderBufferCount, renderBuffers);
		delete[] renderBuffers;
		renderBuffers = nullptr;
	}
	if (FBOFX)
	{
		glDeleteFramebuffers(1, &FBOFX);
		FBOFX = 0;

		glDeleteTextures(renderBufferCount, renderBuffersFX);
		delete[] renderBuffersFX;
		renderBuffersFX = nullptr;
	}
}

void Framebuffer::blitFX()
{
	for (auto& type : this->renderBuffersV)
	{
		if (type.second.attachment != GL_DEPTH_ATTACHMENT)
		{
			glNamedFramebufferReadBuffer(FBO, type.second.attachment);
			glNamedFramebufferDrawBuffer(FBOFX, type.second.attachment);

			glBlitNamedFramebuffer(FBO, FBOFX, 0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		}
		else
		{
			glBlitNamedFramebuffer(FBO, FBOFX, 0, 0, w, h, 0, 0, w, h, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		}
	}
}
