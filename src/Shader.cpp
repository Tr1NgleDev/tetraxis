#include <GL/glew.h>
#include <File.h>
#include <Shader.h>

std::unordered_map<std::string, Shader*> Shader::shaders;
std::unordered_map<std::string, std::vector<Shader::ShaderFile>> Shader::shaderSrcFiles;

Shader::Shader(Shader&& other) noexcept
{
	ID = other.ID;
	uniforms = std::move(other.uniforms);
	name = other.name;

	other.ID = 0;
	other.uniforms = {};
	other.name = "";
}
Shader& Shader::operator=(Shader&& other) noexcept
{
	ID = other.ID;
	uniforms = std::move(other.uniforms);
	name = other.name;

	other.ID = 0;
	other.uniforms = {};
	other.name = "";

	return *this;
}

bool Shader::initSources(const std::vector<Shader::ShaderSource>& sources)
{
	if (ID)
	{
		glDeleteProgram(ID);
		ID = 0;
	}
	uniforms.clear();

	ID = glCreateProgram();

	if (!ID)
		return false;

	std::vector<uint32_t> shaders{};
	shaders.reserve(sources.size());

	bool failed = false;
	int i = 0;
	for (auto& src : sources)
	{
		const char* srcC = src.second.c_str();

		uint32_t shader = glCreateShader(src.first);
		if (!shader)
		{
			for (auto& s : shaders)
				glDeleteShader(s);
			return false;
		}

		glShaderSource(shader, 1, &srcC, nullptr);

		glCompileShader(shader);

		int compileStatus = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

		if (!compileStatus)
		{
			int infoLogLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

			char* infoLog = new char[infoLogLength];

			glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog);

			if (!name.empty() && shaderSrcFiles.contains(name) && shaderSrcFiles[name].size() == sources.size())
				printf("Failed to compile shader \"%s\":\n%s\n", shaderSrcFiles[name][i].second.c_str(), infoLog);
			else
				printf("Failed to compile shader:\n%s\n", infoLog);

			glDeleteShader(shader);

			delete[] infoLog;

			failed = true;
		}
		else
		{
			glAttachShader(ID, shader);
		}

		shaders.emplace_back(shader);
		++i;
	}

	if (!failed)
	{
		glLinkProgram(ID);

		int linkStatus = 0;
		glGetProgramiv(ID, GL_LINK_STATUS, &linkStatus);

		if (!linkStatus)
		{
			int infoLogLength;
			glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &infoLogLength);

			char* infoLog = new char[infoLogLength];

			glGetProgramInfoLog(ID, infoLogLength, nullptr, infoLog);

			if (!name.empty())
				printf("Failed to link shader program \"%s\":\n%s\n", name.c_str(), infoLog);
			else
				printf("Failed to link shader program:\n%s\n", infoLog);

			glDeleteProgram(ID);
			ID = 0;

			delete[] infoLog;

			failed = true;
		}
	}
	else
	{
		glDeleteProgram(ID);
		ID = 0;
	}
	
	for (auto& s : shaders)
		if (s)
			glDeleteShader(s);

	if (!failed && ID)
	{
		int uniformCount = 0;
		glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &uniformCount);

		if (uniformCount)
		{
			uniforms.reserve(uniformCount);

			int maxNameLength = 0;
			int length = 0;
			int count = 0;
			GLenum type = GL_NONE;
			
			glGetProgramiv(ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

			char* uniformName = new char[maxNameLength];

			for (int i = 0; i < uniformCount; ++i)
			{
				glGetActiveUniform(ID, i, maxNameLength, &length, &count, &type, uniformName);

				int location = glGetUniformLocation(ID, uniformName);

				if (location != -1)
					uniforms[std::string(uniformName, length)] = location;
			}

			delete[] uniformName;
		}
	}

	return !failed;
}
bool Shader::initFiles(const std::vector<Shader::ShaderFile>& srcFiles)
{
	if (!name.empty())
		shaderSrcFiles.erase(name);

	std::vector<ShaderSource> sources{};
	sources.reserve(srcFiles.size());
	for (auto& file : srcFiles)
	{
		if (!File::exists(file.second))
		{
			return false;
		}
		sources.emplace_back(file.first, File::readAllText(file.second));
	}

	shaderSrcFiles.insert(std::make_pair(name, srcFiles));
	if (!initSources(sources))
	{
		shaderSrcFiles.erase(name);
		return false;
	}
	return true;
}
bool Shader::reload()
{
	if (name.empty() || !shaderSrcFiles.contains(name)) return false;
	std::vector<Shader::ShaderFile> srcFiles = shaderSrcFiles.at(name);
	return initFiles(std::move(srcFiles));
}

int Shader::getUniformLocation(const std::string& name) const
{
	if (!ID) return -1;

	if (uniforms.contains(name))
		return uniforms.at(name);

	return glGetUniformLocation(ID, name.c_str());
}

int Shader::getUniformLocation(const std::string& name)
{
	if (!ID) return -1;

	if (!uniforms.contains(name))
	{
		int location = glGetUniformLocation(ID, name.c_str());
		if (location != -1)
			uniforms[name] = location;
	}

	return uniforms.at(name);
}

Shader::~Shader()
{
	if (ID)
	{
		glDeleteProgram(ID);
		ID = 0;
		uniforms.clear();

		if (!name.empty() && shaderSrcFiles.contains(name))
			shaderSrcFiles.erase(name);

		name.clear();
	}
}

bool Shader::load(const std::string& name, const std::vector<Shader::ShaderFile>& srcFiles)
{
	if (shaders.contains(name))
		return false;

	Shader* shader = new Shader();
	shader->name = name;
	if (!shader->initFiles(srcFiles))
	{
		delete shader;
		return false;
	}

	shaders[name] = shader;

	return true;
}
Shader* Shader::get(const std::string& name)
{
	if (shaders.contains(name))
		return shaders.at(name);
	return nullptr;
}
std::vector<Shader::ShaderFile> Shader::getSrcFiles(const std::string& name)
{
	if (shaderSrcFiles.contains(name))
		return shaderSrcFiles.at(name);
	return {};
}
void Shader::destroy(const std::string& name)
{
	if (!shaders.contains(name)) return;
	assert(shaderSrcFiles.contains(name));

	delete shaders.at(name);
	shaders.erase(name);
	shaderSrcFiles.erase(name);
}
void Shader::destroy()
{
	for (auto& s : shaders)
		delete s.second;
	shaders.clear();
	shaderSrcFiles.clear();
}

void Shader::setUniform(int loc, int count, const float* v) const
{
	if (!ID) return;
	glProgramUniform1fv(ID, loc, count, v);
}
void Shader::setUniform(int loc, float v) const
{
	if (!ID) return;
	glProgramUniform1f(ID, loc, v);
}
void Shader::setUniform(int loc, const glm::vec2& v) const
{
	if (!ID) return;
	glProgramUniform2fv(ID, loc, 1, &v[0]);
}
void Shader::setUniform(int loc, const glm::vec3& v) const
{
	if (!ID) return;
	glProgramUniform3fv(ID, loc, 1, &v[0]);
}
void Shader::setUniform(int loc, const glm::vec4& v) const
{
	if (!ID) return;
	glProgramUniform4fv(ID, loc, 1, &v[0]);
}
void Shader::setUniform(int loc, bool v) const
{
	if (!ID) return;
	glProgramUniform1i(ID, loc, v);
}
void Shader::setUniform(int loc, int count, const int* v) const
{
	if (!ID) return;
	glProgramUniform1iv(ID, loc, count, v);
}
void Shader::setUniform(int loc, int v) const
{
	if (!ID) return;
	glProgramUniform1i(ID, loc, v);
}
void Shader::setUniform(int loc, const glm::ivec2& v) const
{
	if (!ID) return;
	glProgramUniform2iv(ID, loc, 1, &v[0]);
}
void Shader::setUniform(int loc, const glm::ivec3& v) const
{
	if (!ID) return;
	glProgramUniform3iv(ID, loc, 1, &v[0]);
}
void Shader::setUniform(int loc, const glm::ivec4& v) const
{
	if (!ID) return;
	glProgramUniform4iv(ID, loc, 1, &v[0]);
}
void Shader::setUniform(int loc, int count, const uint32_t* v) const
{
	if (!ID) return;
	glProgramUniform1uiv(ID, loc, count, v);
}
void Shader::setUniform(int loc, uint32_t v) const
{
	if (!ID) return;
	glProgramUniform1ui(ID, loc, v);
}
void Shader::setUniform(int loc, const glm::uvec2& v) const
{
	if (!ID) return;
	glProgramUniform2uiv(ID, loc, 1, &v[0]);
}
void Shader::setUniform(int loc, const glm::uvec3& v) const
{
	if (!ID) return;
	glProgramUniform3uiv(ID, loc, 1, &v[0]);
}
void Shader::setUniform(int loc, const glm::uvec4& v) const
{
	if (!ID) return;
	glProgramUniform4uiv(ID, loc, 1, &v[0]);
}
void Shader::setUniform(int loc, float x, float y) const
{
	if (!ID) return;
	glProgramUniform2f(ID, loc, x, y);
}
void Shader::setUniform(int loc, float x, float y, float z) const
{
	if (!ID) return;
	glProgramUniform3f(ID, loc, x, y, z);
}
void Shader::setUniform(int loc, float x, float y, float z, float w) const
{
	if (!ID) return;
	glProgramUniform4f(ID, loc, x, y, z, w);
}
void Shader::setUniform(int loc, int x, int y) const
{
	if (!ID) return;
	glProgramUniform2i(ID, loc, x, y);
}
void Shader::setUniform(int loc, int x, int y, int z) const
{
	if (!ID) return;
	glProgramUniform3i(ID, loc, x, y, z);
}
void Shader::setUniform(int loc, int x, int y, int z, int w) const
{
	if (!ID) return;
	glProgramUniform4i(ID, loc, x, y, z, w);
}
void Shader::setUniform(int loc, uint32_t x, uint32_t y) const
{
	if (!ID) return;
	glProgramUniform2ui(ID, loc, x, y);
}
void Shader::setUniform(int loc, uint32_t x, uint32_t y, uint32_t z) const
{
	if (!ID) return;
	glProgramUniform3ui(ID, loc, x, y, z);
}
void Shader::setUniform(int loc, uint32_t x, uint32_t y, uint32_t z, uint32_t w) const
{
	if (!ID) return;
	glProgramUniform4ui(ID, loc, x, y, z, w);
}
void Shader::setUniform(int loc, const glm::mat2& v, bool transpose) const
{
	if (!ID) return;
	glProgramUniformMatrix2fv(ID, loc, 1, transpose, &v[0][0]);
}
void Shader::setUniform(int loc, const glm::mat3& v, bool transpose) const
{
	if (!ID) return;
	glProgramUniformMatrix3fv(ID, loc, 1, transpose, &v[0][0]);
}
void Shader::setUniform(int loc, const glm::mat4& v, bool transpose) const
{
	if (!ID) return;
	glProgramUniformMatrix4fv(ID, loc, 1, transpose, &v[0][0]);
}
void Shader::setUniform(int loc, const glm::mat2x3& v, bool transpose) const
{
	if (!ID) return;
	glProgramUniformMatrix2x3fv(ID, loc, 1, transpose, &v[0][0]);
}
void Shader::setUniform(int loc, const glm::mat3x2& v, bool transpose) const
{
	if (!ID) return;
	glProgramUniformMatrix3x2fv(ID, loc, 1, transpose, &v[0][0]);
}
void Shader::setUniform(int loc, const glm::mat2x4& v, bool transpose) const
{
	if (!ID) return;
	glProgramUniformMatrix2x4fv(ID, loc, 1, transpose, &v[0][0]);
}
void Shader::setUniform(int loc, const glm::mat4x2& v, bool transpose) const
{
	if (!ID) return;
	glProgramUniformMatrix4x2fv(ID, loc, 1, transpose, &v[0][0]);
}
void Shader::setUniform(int loc, const glm::mat3x4& v, bool transpose) const
{
	if (!ID) return;
	glProgramUniformMatrix3x4fv(ID, loc, 1, transpose, &v[0][0]);
}
void Shader::setUniform(int loc, const glm::mat4x3& v, bool transpose) const
{
	if (!ID) return;
	glProgramUniformMatrix4x3fv(ID, loc, 1, transpose, &v[0][0]);
}
