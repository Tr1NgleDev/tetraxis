#pragma once

#include <basicIncludes.h>
#include <Shader.h>
#include <Texture.h>
#include <QuadRenderer.h>
#include <TexRenderer.h>
#include <Framebuffer.h>
#include <Mesh.h>
#include <MeshRenderer.h>
#include <Math4D.h>

class Editor
{
private:
	Framebuffer fb;
	glm::mat4 projection2D;
	glm::mat4 projection3D;

	Shader* meshShader;
	Mesh mesh;
	MeshRenderer meshRenderer;

public:
	Editor(int argc, const char* argv[]);
	
	void init(GLFWwindow* window);

	void updateSize(GLFWwindow* window);
	void update(GLFWwindow* window, double dt);
	void render(GLFWwindow* window, double dt);

	void fileDropCallback(GLFWwindow* window, int count, const char* paths[]);
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	void scrollCallback(GLFWwindow* window, double xoff, double yoff);
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void charCallback(GLFWwindow* window, uint32_t codepoint);

	~Editor();
};
