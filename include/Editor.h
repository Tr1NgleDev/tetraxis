#pragma once

#include <iostream>
#include <fstream>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <thread>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Editor
{
private:

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
