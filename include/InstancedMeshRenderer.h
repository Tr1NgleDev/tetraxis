#pragma once

#include "basicIncludes.h"
#include "ShaderStorageBuffer.h"
#include "IMesh.h"
#include "MeshRenderer.h"

class InstancedMeshRenderer : public MeshRenderer
{
public:
	ShaderStorageBuffer SSBO{};

	InstancedMeshRenderer() {}
	InstancedMeshRenderer(const IMesh* mesh);
	void render(const std::vector<void*>& instanceData);
	void render();
	void updateData(const std::vector<void*>& instanceData);
	void updateData(const void* instanceData, int instanceCount);
	void setCount(int instanceCount = 1);
	void setDataSize(int dataSize = 1);
	void cleanup() override;

	InstancedMeshRenderer(InstancedMeshRenderer&& other) noexcept;
	InstancedMeshRenderer& operator=(InstancedMeshRenderer&& other) noexcept;

private:
	int instanceCount = 0;
	int dataSize = 0;
};
