#include <GL/glew.h>
#include <InstancedMeshRenderer.h>

InstancedMeshRenderer::InstancedMeshRenderer(const IMesh* mesh)
	: MeshRenderer(mesh) { }

void InstancedMeshRenderer::render(const std::vector<void*>& instanceData)
{
	updateData(instanceData);

	render();
}

void InstancedMeshRenderer::render()
{
	SSBO.use(0);
	MeshRenderer::render();
}

void InstancedMeshRenderer::updateData(const std::vector<void*>& instanceData)
{
	SSBO.uploadData(dataSize, instanceData);
	this->instanceCount = instanceData.size();
}

void InstancedMeshRenderer::updateData(const void* instanceData, int instanceCount)
{
	SSBO.uploadData(dataSize * instanceCount, instanceData);
	this->instanceCount = instanceCount;
}

void InstancedMeshRenderer::setCount(int instanceCount)
{
	if (this->instanceCount == instanceCount) return;

	this->instanceCount = instanceCount;

	SSBO.resize(dataSize * instanceCount);
}

void InstancedMeshRenderer::setDataSize(int dataSize)
{
	if (this->dataSize == dataSize) return;

	this->dataSize = dataSize;

	setCount(instanceCount);
}

void InstancedMeshRenderer::cleanup()
{
	MeshRenderer::cleanup();
	SSBO.cleanup();
	instanceCount = 0;
	dataSize = 0;
}

InstancedMeshRenderer::InstancedMeshRenderer(InstancedMeshRenderer&& other) noexcept
{
	this->VAO = other.VAO;
	this->VBOs = other.VBOs;
	this->bufferSizes = other.bufferSizes;
	this->indexVBO = other.indexVBO;
	this->vertexCount = other.vertexCount;
	this->bufferCount = other.bufferCount;
	this->mode = other.mode;
	this->instanceCount = other.instanceCount;
	this->dataSize = other.dataSize;

	this->SSBO = std::move(other.SSBO);

	other.VAO = 0;
	other.VBOs = nullptr;
	other.bufferSizes = nullptr;
	other.indexVBO = 0;
	other.vertexCount = 0;
	other.bufferCount = 0;
	other.mode = GL_LINES_ADJACENCY;
	other.instanceCount = 0;
	other.dataSize = 0;
}

InstancedMeshRenderer& InstancedMeshRenderer::operator=(InstancedMeshRenderer&& other) noexcept
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
		this->instanceCount = other.instanceCount;
		this->dataSize = other.dataSize;

		this->SSBO = std::move(other.SSBO);

		other.VAO = 0;
		other.VBOs = nullptr;
		other.bufferSizes = nullptr;
		other.indexVBO = 0;
		other.vertexCount = 0;
		other.bufferCount = 0;
		other.mode = GL_LINES_ADJACENCY;
		other.instanceCount = 0;
		other.dataSize = 0;
	}

	return *this;
}
