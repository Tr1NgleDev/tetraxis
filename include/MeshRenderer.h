#pragma once

#include "basicIncludes.h"
#include "IMesh.h"

class MeshRenderer
{
public:
	uint32_t VAO = 0;
	uint32_t* VBOs = nullptr;
	size_t* bufferSizes = nullptr;
	uint32_t indexVBO = 0;
	int vertexCount = 0;
	int bufferCount = 0;
	uint32_t mode = GL_LINES_ADJACENCY;

	MeshRenderer() {}
	MeshRenderer(const IMesh* mesh);
	virtual ~MeshRenderer();
	void setMesh(const IMesh* mesh);
	void render() const;
	virtual void cleanup();

	MeshRenderer(MeshRenderer&& other) noexcept;
	MeshRenderer& operator=(MeshRenderer&& other) noexcept;

private:
	void init(const IMesh* mesh);
	void initAttrs(const IMesh* mesh);
};
