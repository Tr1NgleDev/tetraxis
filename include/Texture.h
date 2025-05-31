#pragma once

#include <basicIncludes.h>

class Texture
{
public:
	// channels, type
	using ChannelsFormat = std::tuple<int, GLenum>;
	// internalFormat, format, type
	using Format = std::tuple<GLenum, GLenum, GLenum>;

private:
	uint32_t ID = 0;
	GLenum target = 0;

	// [channels, type] -> [internalFormat, format, type] LUT
	static const std::unordered_map<ChannelsFormat, Format> formatsTable;
	static std::unordered_map<std::string, Texture*> textures;
	static std::unordered_map<std::string, std::array<int, 5>> textureSizes;
public:
	Texture() { }
	
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	// 1D
	void writeData(const void* data, Format format, int xOffset, int w);
	// 2D
	void writeData(const void* data, Format format, int xOffset, int yOffset, int w, int h);
	// 3D
	void writeData(const void* data, Format format, int xOffset, int yOffset, int zOffset, int w, int h, int d);
	// 2D_ARRAY
	void writeDataArray(const void* data, Format format, int index, int xOffset, int yOffset, int w, int h);

	// 1D
	bool init(const void* data, Format format, int w, bool linearFilter = false);
	// 2D
	bool init(const void* data, Format format, int w, int h, bool linearFilter = false);
	// 3D
	bool init(const void* data, Format format, int w, int h, int d, bool linearFilter = false);
	// 2D_ARRAY
	bool initArray(const void* data, Format format, int w, int h, int cols, int rows, bool linearFilter = false);

	// 1D
	bool init(const void* data, ChannelsFormat format, int w, bool linearFilter = false);
	// 2D
	bool init(const void* data, ChannelsFormat format, int w, int h, bool linearFilter = false);
	// 3D
	bool init(const void* data, ChannelsFormat format, int w, int h, int d, bool linearFilter = false);
	// 2D_ARRAY
	bool initArray(const void* data, ChannelsFormat format, int w, int h, int cols, int rows, bool linearFilter = false);

	// 1D
	bool init(const uint8_t* data, int channels, int w, bool linearFilter = false);
	// 2D
	bool init(const uint8_t* data, int channels, int w, int h, bool linearFilter = false);
	// 3D
	bool init(const uint8_t* data, int channels, int w, int h, int d, bool linearFilter = false);
	// 2D_ARRAY
	bool initArray(const uint8_t* data, int channels, int w, int h, int cols, int rows, bool linearFilter = false);

	// 1D
	bool init(const float* data, int channels, int w, bool linearFilter = false);
	// 2D
	bool init(const float* data, int channels, int w, int h, bool linearFilter = false);
	// 3D
	bool init(const float* data, int channels, int w, int h, int d, bool linearFilter = false);
	// 2D_ARRAY
	bool initArray(const float* data, int channels, int w, int h, int cols, int rows, bool linearFilter = false);

	~Texture();

	uint32_t id() const { return ID; }
	GLenum getTarget() const { return target; }
	void use(uint32_t unit = 0) const { if (ID) glBindTextureUnit(unit, ID); }

	static bool load(const std::string& path, uint8_t desiredChannels = 0, bool linearFilter = false, const std::string& name = "", bool reload = false);
	static bool loadArray(const std::string& path, int cols, int rows, uint8_t desiredChannels = 0, bool linearFilter = false, const std::string& name = "", bool reload = false);
	static Texture* get(const std::string& name);
	static std::array<int, 5> getSize(const std::string& name);
	static void destroy(const std::string& name);
	static void destroy();
};
