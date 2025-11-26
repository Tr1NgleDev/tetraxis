#include <GL/glew.h>
#include <ShaderStorageBuffer.h>

ShaderStorageBuffer::ShaderStorageBuffer(size_t size, const void* data)
{
	init(size, data);
}

ShaderStorageBuffer::ShaderStorageBuffer(size_t elementSize, const std::vector<void*>& data)
{
	init(elementSize, data);
}

ShaderStorageBuffer::~ShaderStorageBuffer()
{
	cleanup();
}

void ShaderStorageBuffer::init(size_t size, const void* data)
{
	cleanup();

	this->size = size;

	glCreateBuffers(1, &ID);
	glNamedBufferStorage(ID, this->size, nullptr, GL_DYNAMIC_STORAGE_BIT);

	if (data != nullptr)
	{
		uploadData(size, data);
	}
}

void ShaderStorageBuffer::init(size_t elementSize, const std::vector<void*>& data)
{
	cleanup();

	this->size = elementSize * data.size();

	glCreateBuffers(1, &ID);
	glNamedBufferStorage(ID, this->size, nullptr, GL_DYNAMIC_STORAGE_BIT);

	if (!data.empty())
	{
		uploadData(elementSize, data);
	}
}

void ShaderStorageBuffer::uploadData(size_t size, const void* data)
{
	if (data == nullptr) return;

	fit(size);

	glNamedBufferSubData(ID, 0, size, data);
}

void ShaderStorageBuffer::uploadData(size_t elementSize, const std::vector<void*>& data)
{
	if (data.empty()) return;

	size_t totalSize = elementSize * data.size();

	fit(totalSize);

	for (size_t i = 0; i < data.size(); ++i)
	{
		if (data[i] != nullptr)
		{
			glNamedBufferSubData(ID, i * elementSize, elementSize, data[i]);
		}
	}
}

void ShaderStorageBuffer::fit(size_t size)
{
	if (size > this->size)
	{
		init(size, nullptr);
	}
}

void ShaderStorageBuffer::resize(size_t size)
{
	if (size != this->size)
	{
		init(size, nullptr);
	}
}

void ShaderStorageBuffer::cleanup()
{
	if (ID)
	{
		glDeleteBuffers(1, &ID);
		ID = NULL;
		size = 0;
	}
}

ShaderStorageBuffer::ShaderStorageBuffer(ShaderStorageBuffer&& other) noexcept
{
	this->ID = other.ID;
	this->size = other.size;

	other.ID = 0;
	other.size = 0;
}

ShaderStorageBuffer& ShaderStorageBuffer::operator=(ShaderStorageBuffer&& other) noexcept
{
	if (this != &other)
	{
		this->ID = other.ID;
		this->size = other.size;

		other.ID = 0;
		other.size = 0;
	}

	return *this;
}
