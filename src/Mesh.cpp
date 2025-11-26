#include <Mesh.h>

Mesh::Mesh(const fdo::Object& fdo)
{
	if (fdo.isInvalid())
	{
		return;
	}

	fdo::Object obj = fdo;

	std::vector<uint32_t> indices{};
	std::vector<fdo::Point> positions{};
	std::vector<fdo::Point> normals{};
	std::vector<fdo::TexCoord> texCoords{};
	std::vector<fdo::Color> colors{};
	obj.tetrahedralize(indices, &positions, &normals, &texCoords, &colors, false);

	for (size_t i = 0; i < positions.size(); ++i)
	{
		fdo::vec4 vert{ 0.0f, 0.0f, 0.0f, 0.0f };
		fdo::vec4 normal{ 0.0f, 0.0f, 0.0f, 0.0f };
		fdo::u8vec4 col{ 0, 0, 0, 0 };
		fdo::vec3 texCoord{ 0.0f, 0.0f, 0.0f };

		if (i < positions.size())
			vert = positions[i];
		if (i < normals.size())
			normal = normals[i];
		if (i < colors.size())
			col = colors[i];
		if (i < texCoords.size())
			texCoord = texCoords[i];

		vertices.emplace_back(Vertex
			{
				vert,
				normal,
				glm::vec4{ col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, col.a / 255.0f },
				texCoord
			});
	}

	for (size_t i = 0; i < indices.size(); i += 4)
	{
		std::vector<Face> faces
		{
			{ { indices[i + 0], indices[i + 1], indices[i + 2] } },
			{ { indices[i + 0], indices[i + 2], indices[i + 3] } },
			{ { indices[i + 0], indices[i + 3], indices[i + 1] } },
			{ { indices[i + 1], indices[i + 2], indices[i + 3] } },
		};
		cells.emplace_back(faces);
	}

	updateIndices();
}

Mesh::Mesh(const Mesh& other)
{
	this->vertices = other.vertices;
	this->cells = other.cells;
	this->tetIndices = other.tetIndices;
}

Mesh::Mesh(Mesh&& other) noexcept
{
	this->vertices = other.vertices;
	this->cells = other.cells;
	this->tetIndices = other.tetIndices;

	other.vertices.clear();
	other.cells.clear();
	other.tetIndices.clear();
}

Mesh& Mesh::operator=(const Mesh& other)
{
	this->vertices = other.vertices;
	this->cells = other.cells;
	this->tetIndices = other.tetIndices;

	return *this;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
	if (this != &other)
	{
		this->vertices = other.vertices;
		this->cells = other.cells;
		this->tetIndices = other.tetIndices;

		other.vertices.clear();
		other.cells.clear();
		other.tetIndices.clear();
	}

	return *this;
}

void Mesh::updateIndices()
{
	for (auto& cell : cells)
	{
		auto tets = cell.tetrahedralize();
		tetIndices.insert(tetIndices.end(), tets.begin(), tets.end());
	}
}

int Mesh::buffCount() const
{
	return 1;
}

const void* Mesh::buffData(int buffIndex) const
{
	return vertices.data();
}

size_t Mesh::buffSize(int buffIndex) const
{
	return sizeof(Vertex) * vertices.size();
}

int Mesh::attrCount(int buffIndex) const
{
	return 4;
}

uint32_t Mesh::attrType(int buffIndex, int attrIndex) const
{
	return GL_FLOAT;
}

int Mesh::attrSize(int buffIndex, int attrIndex) const
{
	return attrIndex == 3 ? 3 : 4;
}

int Mesh::attrStride(int buffIndex, int attrIndex) const
{
	return sizeof(Vertex);
}

bool Mesh::attrIsInteger(int buffIndex, int attrIndex) const
{
	return false;
}

size_t Mesh::vertCount() const
{
	return tetIndices.size();
}

const void* Mesh::indexBuffData() const
{
	return tetIndices.data();
}

size_t Mesh::indexBuffSize() const
{
	return sizeof(uint32_t) * tetIndices.size();
}

size_t Mesh::attrOffset(int buffIndex, int attrIndex) const
{
	switch (attrIndex)
	{
	case 0: return offsetof(Vertex, position);
	case 1: return offsetof(Vertex, normal);
	case 2: return offsetof(Vertex, color);
	case 3: return offsetof(Vertex, uvw);
	}
	return 0;
}

std::vector<uint32_t> Mesh::Cell::tetrahedralize()
{
	// TODO: proper multi-tet cell tetrahedralization
	// assuming the cell is a tetrahedron already for now.
	// this kinda sucks

	// return empty if not a tet
	if (faces.size() != 4) return {};

	std::vector<uint32_t> result{};
	std::set<uint32_t> resultSet{};
	result.reserve(4);

	uint8_t i = 0;
	for (auto& face : faces)
	{
		for (auto& ind : face.indices)
		{
			if (resultSet.contains(ind)) continue;

			result.emplace_back(ind);
			resultSet.insert(ind);

			if (++i == 4) break;
		}
		if (i == 4) break;
	}

	return result;
}
