#include <GL/glew.h>
#include <Texture.h>

#define NDTF_GL_HELPER_FUNCTIONS
#include <ndtf/ndtf.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(Texture&& other) noexcept
{
	ID = other.ID;
	target = other.target;

	other.ID = 0;
	other.target = 0;
}
Texture& Texture::operator=(Texture&& other) noexcept
{
	ID = other.ID;
	target = other.target;

	other.ID = 0;
	other.target = 0;

	return *this;
}


// 1D
void Texture::writeData(const void* data, Texture::Format format, int xOffset, int w)
{
	if (!ID) return;
	if (!data) return;

	GLenum _internalFormat, _format, _type;
	std::tie(_internalFormat, _format, _type) = format;

	glTextureSubImage1D(ID, 0, xOffset, w, _format, _type, data);
}
// 2D
void Texture::writeData(const void* data, Texture::Format format, int xOffset, int yOffset, int w, int h)
{
	if (!ID) return;
	if (!data) return;

	GLenum _internalFormat, _format, _type;
	std::tie(_internalFormat, _format, _type) = format;

	glTextureSubImage2D(ID, 0, xOffset, yOffset, w, h, _format, _type, data);
}
// 3D
void Texture::writeData(const void* data, Texture::Format format, int xOffset, int yOffset, int zOffset, int w, int h, int d)
{
	if (!ID) return;
	if (!data) return;

	GLenum _internalFormat, _format, _type;
	std::tie(_internalFormat, _format, _type) = format;

	glTextureSubImage3D(ID, 0, xOffset, yOffset, zOffset, w, h, d, _format, _type, data);
}
// 2D_ARRAY
void Texture::writeDataArray(const void* data, Texture::Format format, int index, int xOffset, int yOffset, int w, int h)
{
	if (!ID) return;
	if (!data) return;

	GLenum _internalFormat, _format, _type;
	std::tie(_internalFormat, _format, _type) = format;

	glTextureSubImage3D(ID, 0, xOffset, yOffset, index, w, h, 1, _format, _type, data);
}

// 1D
bool Texture::init(const void* data, Texture::Format format, int w, bool linearFilter)
{
	if (ID)
	{
		glDeleteTextures(1, &ID);
		ID = 0;
	}

	target = GL_TEXTURE_1D;

	GLenum _internalFormat, _format, _type;
	std::tie(_internalFormat, _format, _type) = format;

	glCreateTextures(target, 1, &ID);
	if (!ID) return false;

	glTextureStorage1D(ID, 1, _internalFormat, w);
	writeData(data, format, 0, w);

	glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, linearFilter ? GL_LINEAR : GL_NEAREST);
	glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, linearFilter ? GL_LINEAR : GL_NEAREST);
	glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	return ID;
}
// 2D
bool Texture::init(const void* data, Texture::Format format, int w, int h, bool linearFilter)
{
	if (ID)
	{
		glDeleteTextures(1, &ID);
		ID = 0;
	}

	target = GL_TEXTURE_2D;

	GLenum _internalFormat, _format, _type;
	std::tie(_internalFormat, _format, _type) = format;

	glCreateTextures(target, 1, &ID);
	if (!ID) return false;

	glTextureStorage2D(ID, 1, _internalFormat, w, h);
	writeData(data, format, 0, 0, w, h);

	glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, linearFilter ? GL_LINEAR : GL_NEAREST);
	glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, linearFilter ? GL_LINEAR : GL_NEAREST);
	glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return ID;
}
// 3D
bool Texture::init(const void* data, Texture::Format format, int w, int h, int d, bool linearFilter)
{
	if (ID)
	{
		glDeleteTextures(1, &ID);
		ID = 0;
	}

	target = GL_TEXTURE_3D;

	GLenum _internalFormat, _format, _type;
	std::tie(_internalFormat, _format, _type) = format;

	glCreateTextures(target, 1, &ID);
	if (!ID) return false;

	glTextureStorage3D(ID, 1, _internalFormat, w, h, d);
	writeData(data, format, 0, 0, 0, w, h, d);

	glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, linearFilter ? GL_LINEAR : GL_NEAREST);
	glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, linearFilter ? GL_LINEAR : GL_NEAREST);
	glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return ID;
}
// 2D_ARRAY
bool Texture::initArray(const void* data, Texture::Format format, int w, int h, int cols, int rows, bool linearFilter)
{
	if (ID)
	{
		glDeleteTextures(1, &ID);
		ID = 0;
	}

	target = GL_TEXTURE_2D_ARRAY;

	GLenum _internalFormat, _format, _type;
	std::tie(_internalFormat, _format, _type) = format;

	glCreateTextures(target, 1, &ID);
	if (!ID) return false;

	glTextureStorage3D(ID, 1, _internalFormat, w, h, cols * rows);
	glTextureParameteri(ID, GL_TEXTURE_BASE_LEVEL, 0);
	glTextureParameteri(ID, GL_TEXTURE_MAX_LEVEL, 0);
	glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, linearFilter ? GL_LINEAR : GL_NEAREST);
	glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, linearFilter ? GL_LINEAR : GL_NEAREST);
	glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (data && rows > 0)
	{
		glPixelStorei(GL_UNPACK_ROW_LENGTH, w);

		for (int col = 0; col < cols; ++col)
		{
			for (int row = 0; row < rows; ++row)
			{
				glPixelStorei(GL_UNPACK_SKIP_PIXELS, w * col);
				glPixelStorei(GL_UNPACK_SKIP_ROWS, h * row);
				writeData(data, format, row * cols + col, 0, 0, w, h, 1);
			}
		}

		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	}

	return ID;
}

// 1D
bool Texture::init(const void* data, Texture::ChannelsFormat format, int w, bool linearFilter)
{
	if (!formatsTable.contains(format))
		return false;
	return init(data, formatsTable.at(format), w, linearFilter);
}
// 2D
bool Texture::init(const void* data, Texture::ChannelsFormat format, int w, int h, bool linearFilter)
{
	if (!formatsTable.contains(format))
		return false;
	return init(data, formatsTable.at(format), w, h, linearFilter);
}
// 3D
bool Texture::init(const void* data, Texture::ChannelsFormat format, int w, int h, int d, bool linearFilter)
{
	if (!formatsTable.contains(format))
		return false;
	return init(data, formatsTable.at(format), w, h, d, linearFilter);
}
// 2D_ARRAY
bool Texture::initArray(const void* data, Texture::ChannelsFormat format, int w, int h, int cols, int rows, bool linearFilter)
{
	if (!formatsTable.contains(format))
		return false;
	return initArray(data, formatsTable.at(format), w, h, cols, rows, linearFilter);
}

// 1D
bool Texture::init(const uint8_t* data, int channels, int w, bool linearFilter)
{
	return init(data, ChannelsFormat{ channels, GL_UNSIGNED_BYTE }, w, linearFilter);
}
// 2D
bool Texture::init(const uint8_t* data, int channels, int w, int h, bool linearFilter)
{
	return init(data, ChannelsFormat{ channels, GL_UNSIGNED_BYTE }, w, h, linearFilter);
}
// 3D
bool Texture::init(const uint8_t* data, int channels, int w, int h, int d, bool linearFilter)
{
	return init(data, ChannelsFormat{ channels, GL_UNSIGNED_BYTE }, w, h, d, linearFilter);
}
// 2D_ARRAY
bool Texture::initArray(const uint8_t* data, int channels, int w, int h, int cols, int rows, bool linearFilter)
{
	return initArray(data, ChannelsFormat{ channels, GL_UNSIGNED_BYTE }, w, h, cols, rows, linearFilter);
}

// 1D
bool Texture::init(const float* data, int channels, int w, bool linearFilter)
{
	return init(data, ChannelsFormat{ channels, GL_FLOAT }, w, linearFilter);
}
// 2D
bool Texture::init(const float* data, int channels, int w, int h, bool linearFilter)
{
	return init(data, ChannelsFormat{ channels, GL_FLOAT }, w, h, linearFilter);
}
// 3D
bool Texture::init(const float* data, int channels, int w, int h, int d, bool linearFilter)
{
	return init(data, ChannelsFormat{ channels, GL_FLOAT }, w, h, d, linearFilter);
}
// 2D_ARRAY
bool Texture::initArray(const float* data, int channels, int w, int h, int cols, int rows, bool linearFilter)
{
	return initArray(data, ChannelsFormat{ channels, GL_FLOAT }, w, h, cols, rows, linearFilter);
}

Texture::~Texture()
{
	if (ID)
	{
		glDeleteTextures(1, &ID);
		ID = 0;
		target = 0;
	}
}

// [channels, type] -> [internalFormat, format, type] LUT
const std::unordered_map<Texture::ChannelsFormat, Texture::Format> Texture::formatsTable
{
	{ { 1, GL_UNSIGNED_BYTE },		{ GL_R8,		GL_RED,		GL_UNSIGNED_BYTE	} },
	{ { 1, GL_BYTE },				{ GL_R8,		GL_RED,		GL_BYTE				} },
	{ { 1, GL_UNSIGNED_SHORT },		{ GL_R16,		GL_RED,		GL_UNSIGNED_SHORT	} },
	{ { 1, GL_SHORT },				{ GL_R16,		GL_RED,		GL_SHORT			} },
	{ { 1, GL_UNSIGNED_INT },		{ GL_R32F,		GL_RED,		GL_UNSIGNED_INT		} },
	{ { 1, GL_INT },				{ GL_R32F,		GL_RED,		GL_INT				} },
	{ { 1, GL_FLOAT },				{ GL_R32F,		GL_RED,		GL_FLOAT			} },
	{ { 1, GL_HALF_FLOAT },			{ GL_R16F,		GL_RED,		GL_HALF_FLOAT		} },

	{ { 2, GL_UNSIGNED_BYTE },		{ GL_RG8,		GL_RG,		GL_UNSIGNED_BYTE	} },
	{ { 2, GL_BYTE },				{ GL_RG8,		GL_RG,		GL_BYTE				} },
	{ { 2, GL_UNSIGNED_SHORT },		{ GL_RG16,		GL_RG,		GL_UNSIGNED_SHORT	} },
	{ { 2, GL_SHORT },				{ GL_RG16,		GL_RG,		GL_SHORT			} },
	{ { 2, GL_UNSIGNED_INT },		{ GL_RG32F,		GL_RG,		GL_UNSIGNED_INT		} },
	{ { 2, GL_INT },				{ GL_RG32F,		GL_RG,		GL_INT				} },
	{ { 2, GL_FLOAT },				{ GL_RG32F,		GL_RG,		GL_FLOAT			} },
	{ { 2, GL_HALF_FLOAT },			{ GL_RG16F,		GL_RG,		GL_HALF_FLOAT		} },

	{ { 3, GL_UNSIGNED_BYTE },		{ GL_RGB8,		GL_RGB,		GL_UNSIGNED_BYTE	} },
	{ { 3, GL_BYTE },				{ GL_RGB8,		GL_RGB,		GL_BYTE				} },
	{ { 3, GL_UNSIGNED_SHORT },		{ GL_RGB16,		GL_RGB,		GL_UNSIGNED_SHORT	} },
	{ { 3, GL_SHORT },				{ GL_RGB16,		GL_RGB,		GL_SHORT			} },
	{ { 3, GL_UNSIGNED_INT },		{ GL_RGB32F,	GL_RGB,		GL_UNSIGNED_INT		} },
	{ { 3, GL_INT },				{ GL_RGB32F,	GL_RGB,		GL_INT				} },
	{ { 3, GL_FLOAT },				{ GL_RGB32F,	GL_RGB,		GL_FLOAT			} },
	{ { 3, GL_HALF_FLOAT },			{ GL_RGB16F,	GL_RGB,		GL_HALF_FLOAT		} },

	{ { 4, GL_UNSIGNED_BYTE },		{ GL_RGBA8,		GL_RGBA,	GL_UNSIGNED_BYTE	} },
	{ { 4, GL_BYTE },				{ GL_RGBA8,		GL_RGBA,	GL_BYTE				} },
	{ { 4, GL_UNSIGNED_SHORT },		{ GL_RGBA16,	GL_RGBA,	GL_UNSIGNED_SHORT	} },
	{ { 4, GL_SHORT },				{ GL_RGBA16,	GL_RGBA,	GL_SHORT			} },
	{ { 4, GL_UNSIGNED_INT },		{ GL_RGBA32F,	GL_RGBA,	GL_UNSIGNED_INT		} },
	{ { 4, GL_INT },				{ GL_RGBA32F,	GL_RGBA,	GL_INT				} },
	{ { 4, GL_FLOAT },				{ GL_RGBA32F,	GL_RGBA,	GL_FLOAT			} },
	{ { 4, GL_HALF_FLOAT },			{ GL_RGBA16F,	GL_RGBA,	GL_HALF_FLOAT		} },
};

std::unordered_map<std::string, Texture*> Texture::textures{};
std::unordered_map<std::string, std::array<int, 5>> Texture::textureSizes{};

bool Texture::load(const std::string& path, uint8_t desiredChannels, bool linearFilter, const std::string& _name, bool reload)
{
	std::string name = _name;
	if (name.empty()) name = path;

	if (textures.contains(name))
	{
		if (reload)
		{
			destroy(name);
		}
		else
		{
			return false;
		}
	}

	assert(!textures.contains(name));
	assert(!textureSizes.contains(name));

	NDTF_File ndtf = ndtf_file_load(path.c_str(), nullptr, NDTF_TEXELFORMAT_NONE);
	if (ndtf_file_isValid(&ndtf)) // load the ndtf
	{
		int channels = ndtf_getChannelCount((NDTF_TexelFormat)ndtf.header.texelFormat);
		if (desiredChannels && channels != desiredChannels)
		{
			if (desiredChannels == 2) // ndtf doesn't support RG channels atm
			{
				ndtf_file_free(&ndtf);
				return false;
			}

			switch (channels)
			{
			case 4:
			case 3:
			{
				int newFormat = ndtf.header.texelFormat;
				newFormat += channels - desiredChannels;
				if (desiredChannels == 1)
					newFormat -= 1;
				ndtf_file_reformat(&ndtf, (NDTF_TexelFormat)newFormat);
			} break;
			case 1:
			{
				int newFormat = ndtf.header.texelFormat;
				newFormat += channels - desiredChannels;
				newFormat += 1;
				ndtf_file_reformat(&ndtf, (NDTF_TexelFormat)newFormat);
			} break;
			}
		}

		Texture* tex = new Texture();
		if (ndtf.header.dimensions == NDTF_DIMENSIONS_TWO)
		{
			if (!tex->init(
				ndtf.data,
				Format
				{
					ndtf_glSizedTexelFormat((NDTF_TexelFormat)ndtf.header.texelFormat),
					ndtf_glTexelFormat((NDTF_TexelFormat)ndtf.header.texelFormat),
					ndtf_glTexelType((NDTF_TexelFormat)ndtf.header.texelFormat)
				},
				ndtf.header.width,
				ndtf.header.height,
				linearFilter
			))
			{
				delete tex;
				ndtf_file_free(&ndtf);
				return false;
			}
		}
		else
		{
			if (!tex->init(
				ndtf.data,
				Format
				{
					ndtf_glSizedTexelFormat((NDTF_TexelFormat)ndtf.header.texelFormat),
					ndtf_glTexelFormat((NDTF_TexelFormat)ndtf.header.texelFormat),
					ndtf_glTexelType((NDTF_TexelFormat)ndtf.header.texelFormat)
				},
				ndtf.header.width,
				ndtf.header.height,
				ndtf.header.depth * ndtf.header.ind * ndtf.header.ind2,
				linearFilter
			))
			{
				delete tex;
				ndtf_file_free(&ndtf);
				return false;
			}
		}

		textures[name] = tex;
		textureSizes[name] =
			{ (int)ndtf.header.width, (int)ndtf.header.height, (int)ndtf.header.depth, (int)ndtf.header.ind, (int)ndtf.header.ind2 };

		ndtf_file_free(&ndtf);
		return true;
	}
	else // use stb_image
	{
		int w, h, c;
		stbi_set_flip_vertically_on_load(true);
		uint8_t* data = stbi_load(path.c_str(), &w, &h, &c, desiredChannels);

		if (!data)
		{
			return false;
		}

		Texture* tex = new Texture();
		if (!tex->init(
			data,
			desiredChannels ? desiredChannels : c,
			w,
			h,
			linearFilter
		))
		{
			delete tex;
			stbi_image_free(data);
			return false;
		}

		textures[name] = tex;
		textureSizes[name] =
		{ w, h, 1, 1, 1 };

		stbi_image_free(data);
		return true;
	}

	return false;
}

bool Texture::loadArray(const std::string& path, int cols, int rows, uint8_t desiredChannels, bool linearFilter, const std::string& _name, bool reload)
{
	std::string name = _name;
	if (name.empty()) name = path;

	if (textures.contains(name))
	{
		if (reload)
		{
			destroy(name);
		}
		else
		{
			return false;
		}
	}

	assert(!textures.contains(name));
	assert(!textureSizes.contains(name));

	int w, h, c;
	stbi_set_flip_vertically_on_load(true);
	uint8_t* data = stbi_load(path.c_str(), &w, &h, &c, desiredChannels);

	if (!data)
	{
		return false;
	}

	Texture* tex = new Texture();
	if (!tex->initArray(
		data,
		desiredChannels ? desiredChannels : c,
		w,
		h,
		cols,
		rows,
		linearFilter
	))
	{
		delete tex;
		stbi_image_free(data);
		return false;
	}

	textures[name] = tex;
	textureSizes[name] =
	{ w / cols, h / rows, cols, rows, 1 };

	stbi_image_free(data);
	return true;
}

Texture* Texture::get(const std::string& name)
{
	if (!textures.contains(name)) return nullptr;
	assert(textureSizes.contains(name));

	return textures.at(name);
}

std::array<int, 5> Texture::getSize(const std::string& name)
{
	if (!textures.contains(name)) return { 1, 1, 1, 1, 1 };
	assert(textureSizes.contains(name));

	return textureSizes.at(name);
}

void Texture::destroy(const std::string& name)
{
	if (!textures.contains(name)) return;
	assert(textureSizes.contains(name));

	delete textures.at(name);
	textures.erase(name);
	textureSizes.erase(name);
}

void Texture::destroy()
{
	for (auto& tex : textures)
		delete tex.second;
	textures.clear();
	textureSizes.clear();
}
