#pragma once

#include <basicIncludes.h>

class Shader
{
public:
	using ShaderSource = std::pair<GLenum, std::string>;
	using ShaderFile = std::pair<GLenum, std::string>;

	static std::unordered_map<std::string, Shader*> shaders;
	static std::unordered_map<std::string, std::vector<ShaderFile>> shaderSrcFiles;

private:
	uint32_t ID = 0;
	std::unordered_map<std::string, int> uniforms{};
	std::string name = "";

public:
	Shader() { }

	uint32_t id() const { return ID; }
	void use() const { if (ID) glUseProgram(ID); }

	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;

	bool initSources(const std::vector<ShaderSource>& sources);
	bool initFiles(const std::vector<ShaderFile>& srcFiles);
	bool reload();

	int getUniformLocation(const std::string& name) const;
	int getUniformLocation(const std::string& name);

	~Shader();

	static bool load(const std::string& name, const std::vector<ShaderFile>& srcFiles);
	static Shader* get(const std::string& name);
	static std::vector<ShaderFile> getSrcFiles(const std::string& name);
	static void destroy(const std::string& name);
	static void destroy();

	void setUniform(int loc, int count, const float* v) const;
	void setUniform(int loc, float v) const;
	void setUniform(int loc, const glm::vec2& v) const;
	void setUniform(int loc, const glm::vec3& v) const;
	void setUniform(int loc, const glm::vec4& v) const;
	void setUniform(int loc, bool v) const;
	void setUniform(int loc, int count, const int* v) const;
	void setUniform(int loc, int v) const;
	void setUniform(int loc, const glm::ivec2& v) const;
	void setUniform(int loc, const glm::ivec3& v) const;
	void setUniform(int loc, const glm::ivec4& v) const;
	void setUniform(int loc, int count, const uint32_t* v) const;
	void setUniform(int loc, uint32_t v) const;
	void setUniform(int loc, const glm::uvec2& v) const;
	void setUniform(int loc, const glm::uvec3& v) const;
	void setUniform(int loc, const glm::uvec4& v) const;
	void setUniform(int loc, float x, float y) const;
	void setUniform(int loc, float x, float y, float z) const;
	void setUniform(int loc, float x, float y, float z, float w) const;
	void setUniform(int loc, int x, int y) const;
	void setUniform(int loc, int x, int y, int z) const;
	void setUniform(int loc, int x, int y, int z, int w) const;
	void setUniform(int loc, uint32_t x, uint32_t y) const;
	void setUniform(int loc, uint32_t x, uint32_t y, uint32_t z) const;
	void setUniform(int loc, uint32_t x, uint32_t y, uint32_t z, uint32_t w) const;
	void setUniform(int loc, const glm::mat2& v, bool transpose = false) const;
	void setUniform(int loc, const glm::mat3& v, bool transpose = false) const;
	void setUniform(int loc, const glm::mat4& v, bool transpose = false) const;
	void setUniform(int loc, const glm::mat2x3& v, bool transpose = false) const;
	void setUniform(int loc, const glm::mat3x2& v, bool transpose = false) const;
	void setUniform(int loc, const glm::mat2x4& v, bool transpose = false) const;
	void setUniform(int loc, const glm::mat4x2& v, bool transpose = false) const;
	void setUniform(int loc, const glm::mat3x4& v, bool transpose = false) const;
	void setUniform(int loc, const glm::mat4x3& v, bool transpose = false) const;
	//void setUniform(int loc, const m4::Mat5& v) const;
	//void setUniform(int loc, const m4::BiVector4& v) const;
	//void setUniform(int loc, const m4::vec5& v) const;

	void setUniform(const std::string& name, int count, const float* v) const
	{
		setUniform(getUniformLocation(name), count, v);
	}
	void setUniform(const std::string& name, float v) const
	{
		setUniform(getUniformLocation(name), v);
	}
	void setUniform(const std::string& name, const glm::vec2& v) const
	{
		setUniform(getUniformLocation(name), v);
	}
	void setUniform(const std::string& name, const glm::vec3& v) const
	{
		setUniform(getUniformLocation(name), v);
	}
	void setUniform(const std::string& name, const glm::vec4& v) const
	{
		setUniform(getUniformLocation(name), v);
	}
	void setUniform(const std::string& name, bool v) const
	{
		setUniform(getUniformLocation(name), v);
	}
	void setUniform(const std::string& name, int count, const int* v) const
	{
		setUniform(getUniformLocation(name), count, v);
	}
	void setUniform(const std::string& name, int v) const
	{
		setUniform(getUniformLocation(name), v);
	}
	void setUniform(const std::string& name, const glm::ivec2& v) const
	{
		setUniform(getUniformLocation(name), v);
	}
	void setUniform(const std::string& name, const glm::ivec3& v) const
	{
		setUniform(getUniformLocation(name), v);
	}
	void setUniform(const std::string& name, const glm::ivec4& v) const
	{
		setUniform(getUniformLocation(name), v);
	}
	void setUniform(const std::string& name, int count, const uint32_t* v) const
	{
		setUniform(getUniformLocation(name), count, v);
	}
	void setUniform(const std::string& name, uint32_t v) const
	{
		setUniform(getUniformLocation(name), v);
	}
	void setUniform(const std::string& name, const glm::uvec2& v) const
	{
		setUniform(getUniformLocation(name), v);
	}
	void setUniform(const std::string& name, const glm::uvec3& v) const
	{
		setUniform(getUniformLocation(name), v);
	}
	void setUniform(const std::string& name, const glm::uvec4& v) const
	{
		setUniform(getUniformLocation(name), v);
	}
	void setUniform(const std::string& name, float x, float y) const
	{
		setUniform(getUniformLocation(name), x, y);
	}
	void setUniform(const std::string& name, float x, float y, float z) const
	{
		setUniform(getUniformLocation(name), x, y, z);
	}
	void setUniform(const std::string& name, float x, float y, float z, float w) const
	{
		setUniform(getUniformLocation(name), x, y, z, w);
	}
	void setUniform(const std::string& name, int x, int y) const
	{
		setUniform(getUniformLocation(name), x, y);
	}
	void setUniform(const std::string& name, int x, int y, int z) const
	{
		setUniform(getUniformLocation(name), x, y, z);
	}
	void setUniform(const std::string& name, int x, int y, int z, int w) const
	{
		setUniform(getUniformLocation(name), x, y, z, w);
	}
	void setUniform(const std::string& name, uint32_t x, uint32_t y) const
	{
		setUniform(getUniformLocation(name), x, y);
	}
	void setUniform(const std::string& name, uint32_t x, uint32_t y, uint32_t z) const
	{
		setUniform(getUniformLocation(name), x, y, z);
	}
	void setUniform(const std::string& name, uint32_t x, uint32_t y, uint32_t z, uint32_t w) const
	{
		setUniform(getUniformLocation(name), x, y, z, w);
	}
	void setUniform(const std::string& name, const glm::mat2& v, bool transpose = false) const
	{
		setUniform(getUniformLocation(name), v, transpose);
	}
	void setUniform(const std::string& name, const glm::mat3& v, bool transpose = false) const
	{
		setUniform(getUniformLocation(name), v, transpose);
	}
	void setUniform(const std::string& name, const glm::mat4& v, bool transpose = false) const
	{
		setUniform(getUniformLocation(name), v, transpose);
	}
	void setUniform(const std::string& name, const glm::mat2x3& v, bool transpose = false) const
	{
		setUniform(getUniformLocation(name), v, transpose);
	}
	void setUniform(const std::string& name, const glm::mat3x2& v, bool transpose = false) const
	{
		setUniform(getUniformLocation(name), v, transpose);
	}
	void setUniform(const std::string& name, const glm::mat2x4& v, bool transpose = false) const
	{
		setUniform(getUniformLocation(name), v, transpose);
	}
	void setUniform(const std::string& name, const glm::mat4x2& v, bool transpose = false) const
	{
		setUniform(getUniformLocation(name), v, transpose);
	}
	void setUniform(const std::string& name, const glm::mat3x4& v, bool transpose = false) const
	{
		setUniform(getUniformLocation(name), v, transpose);
	}
	void setUniform(const std::string& name, const glm::mat4x3& v, bool transpose = false) const
	{
		setUniform(getUniformLocation(name), v, transpose);
	}
	//void setUniform(const std::string& name, const m4::Mat5& v) const
	//{
	//	setUniform(getUniformLocation(name), v);
	//}
	//void setUniform(const std::string& name, const m4::BiVector4& v) const
	//{
	//	setUniform(getUniformLocation(name), v);
	//}
	//void setUniform(const std::string& name, const m4::vec5& v) const
	//{
	//	setUniform(getUniformLocation(name), v);
	//}
};
