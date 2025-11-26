#include <GL/glew.h>
#include <MeshRenderer.h>

MeshRenderer::MeshRenderer(const IMesh* mesh)
{
	init(mesh);
}

MeshRenderer::~MeshRenderer()
{
	cleanup();
}

void MeshRenderer::setMesh(const IMesh* mesh)
{
	init(mesh);
}

void MeshRenderer::init(const IMesh* mesh)
{
	if (VAO)
	{
		if (mesh->vertCount() == 0)
		{
			cleanup();
		}
		else
		{
			assert(mesh->buffCount() == bufferCount);

			if (!bufferSizes)
			{
				bufferSizes = new size_t[bufferCount + 1](0);
			}

			if (bufferCount > 0)
			{
				for (int buff = 0; buff < bufferCount; ++buff)
				{
					int newSize = mesh->buffSize(buff);
					if (bufferSizes[buff + 1] >= newSize)
						glNamedBufferSubData(VBOs[buff], 0, newSize, mesh->buffData(buff));
					else
						glNamedBufferData(VBOs[buff], newSize, mesh->buffData(buff), GL_DYNAMIC_DRAW);
					bufferSizes[buff + 1] = newSize;
				}
			}
			vertexCount = mesh->vertCount();

			const void* data = mesh->indexBuffData();

			if (data)
			{
				assert(indexVBO != 0);

				int newSize = mesh->indexBuffSize();
				if (bufferSizes[0] >= newSize)
					glNamedBufferSubData(indexVBO, 0, newSize, data);
				else
					glNamedBufferData(indexVBO, newSize, data, GL_DYNAMIC_DRAW);
				bufferSizes[0] = newSize;
			}
		}
	}
	else
	{
		cleanup();

		if (mesh->vertCount() > 0)
		{
			glCreateVertexArrays(1, &VAO);

			bufferCount = mesh->buffCount();

			VBOs = new uint32_t[bufferCount];

			if (bufferSizes)
			{
				delete[] bufferSizes;
			}
			bufferSizes = new size_t[bufferCount + 1](0);

			glCreateBuffers(bufferCount, VBOs);
			initAttrs(mesh);

			const void* indexData = mesh->indexBuffData();
			if (indexData)
			{
				int size = mesh->indexBuffSize();
				bufferSizes[0] = size;

				glCreateBuffers(1, &indexVBO);
				glNamedBufferData(indexVBO, size, indexData, GL_STATIC_DRAW);
				glVertexArrayElementBuffer(VAO, indexVBO);
			}

			vertexCount = mesh->vertCount();
		}
	}
}

void MeshRenderer::render() const
{
	if (!vertexCount) return;

	glBindVertexArray(VAO);

	if (indexVBO)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
		glDrawElements(mode, vertexCount, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
	{
		glDrawArrays(mode, 0, vertexCount);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshRenderer::cleanup()
{
	if (bufferSizes)
	{
		delete[] bufferSizes;
	}

	if (VBOs)
	{
		glDeleteBuffers(bufferCount, VBOs);
		delete[] VBOs;
		VBOs = nullptr;
	}

	if (indexVBO)
	{
		glDeleteBuffers(1, &indexVBO);
		indexVBO = 0;
	}

	if (VAO)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	vertexCount = 0;
	bufferCount = 0;
}


MeshRenderer::MeshRenderer(MeshRenderer&& other) noexcept
{
	this->VAO = other.VAO;
	this->VBOs = other.VBOs;
	this->bufferSizes = other.bufferSizes;
	this->indexVBO = other.indexVBO;
	this->vertexCount = other.vertexCount;
	this->bufferCount = other.bufferCount;
	this->mode = other.mode;

	other.VAO = 0;
	other.VBOs = nullptr;
	other.bufferSizes = nullptr;
	other.indexVBO = 0;
	other.vertexCount = 0;
	other.bufferCount = 0;
	other.mode = GL_LINES_ADJACENCY;
}

MeshRenderer& MeshRenderer::operator=(MeshRenderer&& other) noexcept
{
	if (this != &other)
	{
		this->VAO = other.VAO;
		this->VBOs = other.VBOs;
		this->bufferSizes = other.bufferSizes;
		this->indexVBO = other.indexVBO;
		this->vertexCount = other.vertexCount;
		this->bufferCount = other.bufferCount;
		this->mode = other.mode;

		other.VAO = 0;
		other.VBOs = nullptr;
		other.bufferSizes = nullptr;
		other.indexVBO = 0;
		other.vertexCount = 0;
		other.bufferCount = 0;
		other.mode = GL_LINES_ADJACENCY;
	}

	return *this;
}

void MeshRenderer::initAttrs(const IMesh* mesh)
{
	int totalAttrCount = 0;
	for (int i = 0; i < bufferCount; ++i)
	{
		int size = mesh->buffSize(i);
		bufferSizes[i + 1] = size;
		glNamedBufferData(VBOs[i], size, mesh->buffData(i), GL_STATIC_DRAW);

		int attrCount = mesh->attrCount(i);

		for (int j = 0; j < attrCount; ++j)
		{
			size_t size = mesh->attrSize(i, j);
			GLenum type = mesh->attrType(i, j);
			int stride = mesh->attrStride(i, j);
			size_t off = mesh->attrOffset(i, j);
			bool isInteger = mesh->attrIsInteger(i, j);

			glEnableVertexArrayAttrib(VAO, totalAttrCount);
			glVertexArrayVertexBuffer(VAO, totalAttrCount, VBOs[i], off, stride);
			if (isInteger)
			{
				glVertexArrayAttribIFormat(VAO, totalAttrCount,
					size,
					type,
					0);
			}
			else
			{
				glVertexArrayAttribFormat(VAO, totalAttrCount,
					size,
					type,
					GL_FALSE,
					0);
			}
			
			glVertexArrayAttribBinding(VAO, totalAttrCount, totalAttrCount);

			++totalAttrCount;
		}
	}
}
