#pragma once

#include "basicIncludes.h"
#include "IMesh.h"
#include <4do/4do.h>

class Mesh : public IMesh
{
public:
	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec4 color;
		glm::vec3 uvw;
	};

	struct Face
	{
		std::vector<uint32_t> indices;
	};

	struct Cell
	{
		std::vector<Face> faces;

		std::vector<uint32_t> tetrahedralize();
	};

	std::vector<Vertex> vertices{};
	std::vector<Cell> cells{};

	std::vector<uint32_t> tetIndices{};

	Mesh() {}
	Mesh(const fdo::Object& fdo);

	Mesh(const Mesh& other);
	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(const Mesh& other);
	Mesh& operator=(Mesh&& other) noexcept;

	void updateIndices();

	int buffCount() const override;
	const void* buffData(int buffIndex) const override;
	size_t buffSize(int buffIndex) const override;
	int attrCount(int buffIndex) const override;
	uint32_t attrType(int buffIndex, int attrIndex) const override;
	int attrSize(int buffIndex, int attrIndex) const override;
	int attrStride(int buffIndex, int attrIndex) const override;
	size_t attrOffset(int buffIndex, int attrIndex) const override;
	bool attrIsInteger(int buffIndex, int attrIndex) const override;
	size_t vertCount() const override;
	const void* indexBuffData() const override;
	size_t indexBuffSize() const override;
};
