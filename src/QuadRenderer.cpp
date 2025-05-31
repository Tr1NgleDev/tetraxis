#include <GL/glew.h>
#include <QuadRenderer.h>

uint32_t QuadRenderer::VAO = 0;
uint32_t QuadRenderer::VBO = 0;

void QuadRenderer::init()
{
	if (VAO != NULL && VBO != NULL) return;

	glCreateBuffers(1, &VBO);
	glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glCreateVertexArrays(1, &VAO);
	glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 2 * sizeof(uint8_t));

	glEnableVertexArrayAttrib(VAO, 0);
	glVertexArrayAttribFormat(VAO, 0, 2, GL_UNSIGNED_BYTE, GL_FALSE, 0);
	glVertexArrayAttribBinding(VAO, 0, 0);
}

void QuadRenderer::render(GLenum mode)
{
	if (VAO)
	{
		glBindVertexArray(VAO);
		glDrawArrays(mode, 0, 4);
	}
}

void QuadRenderer::destroy()
{
	if (VAO)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	if (VBO)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
}
