#pragma once

#include "basicIncludes.h"

class ShaderStorageBuffer
{
private:
	uint32_t ID = NULL;
	size_t size = 0;

	void init(size_t size, const void* data = nullptr);
	void init(size_t elementSize, const std::vector<void*>& data);

public:
	ShaderStorageBuffer() {}
	ShaderStorageBuffer(size_t size, const void* data = nullptr);
	ShaderStorageBuffer(size_t elementSize, const std::vector<void*>& data);

	void uploadData(size_t size, const void* data = nullptr);
	void uploadData(size_t elementSize, const std::vector<void*>& data);

	// might clear the data! use with caution!
	void fit(size_t size);

	// might clear the data! use with caution!
	void resize(size_t size);

	void cleanup();

	uint32_t id() const
	{
		return ID;
	}

	void use(uint32_t index) const
	{
		if (ID)
		{
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, ID);
		}
	}

	ShaderStorageBuffer(ShaderStorageBuffer&& other) noexcept;
	ShaderStorageBuffer& operator=(ShaderStorageBuffer&& other) noexcept;

	~ShaderStorageBuffer();
};
