#pragma once

#include <basicIncludes.h>
#include <Shader.h>
#include <Texture.h>
#include <QuadRenderer.h>
//#include <qsvgrenderer.h>

class Editor
{
private:
	Shader* testShader;
	Texture* tex;
	//Texture svgTex;
	//QSvgRenderer svgRenderer{};

public:
	Editor(int argc, const char* argv[]);
	
	void initRender(GLFWwindow* window);

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
