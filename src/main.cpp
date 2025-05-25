#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <thread>

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdint>

#include <Editor.h>

GLFWwindow* window = nullptr;
Editor* editor = nullptr;
std::atomic<bool> renderWindowResized = true;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	editor->keyCallback(window, key, scancode, action, mods);
}
void charCallback(GLFWwindow* window, uint32_t codepoint)
{
	editor->charCallback(window, codepoint);
}
void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	editor->mouseCallback(window, xpos, ypos);
}
void scrollCallback(GLFWwindow* window, double xoff, double yoff)
{
	editor->scrollCallback(window, xoff, yoff);
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	editor->mouseButtonCallback(window, button, action, mods);
}
void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	renderWindowResized = true;
}
void fileDropCallback(GLFWwindow* window, int count, const char* paths[])
{
	editor->fileDropCallback(window, count, paths);
}

int main(int argc, const char* argv[])
{
	if (!glfwInit())
	{
		printf("Failed to initialize GLFW!\n");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
#endif
	const char* sessionType = std::getenv("XDG_SESSION_TYPE");
	if (sessionType && std::string(sessionType) == "wayland")
		glfwWindowHintString(GLFW_WAYLAND_APP_ID, "tr1ngledev.tetraxis");

	window = glfwCreateWindow(1280, 720, "Tetraxis", nullptr, nullptr);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, 1);
	glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, 1);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetDropCallback(window, fileDropCallback);
	glfwSetFramebufferSizeCallback(window, windowResizeCallback);
	glfwSetCharCallback(window, charCallback);

	editor = new Editor(argc, argv);
	glfwSetWindowUserPointer(window, editor);

	std::atomic<bool> renderThreadStop = false;
	std::thread renderThread(
		[&renderThreadStop, &argc, &argv]()
		{
			glfwMakeContextCurrent(window);
			glewExperimental = true;
			if (glewInit())
			{
				printf("Failed to initialize GLEW!\n");
				return 2;
			}
			
			glfwSwapInterval(1);

			editor->initRender(window);
			editor->updateSize(window);

			double lastTime = glfwGetTime() - 0.01;
			while (!renderThreadStop)
			{
				double time = glfwGetTime();
				double dt = glm::min(time - lastTime, 1.0 / 10.0);
				lastTime = time;

				if (renderWindowResized)
				{
					renderWindowResized = false;
					editor->updateSize(window);
				}

				editor->render(window, dt);

				glfwSwapBuffers(window);
			}
			renderThreadStop = false;
		});
	renderThread.detach();

	double lastTime = glfwGetTime() - 0.01;
	while (!glfwWindowShouldClose(window))
	{
		double time = glfwGetTime();
		double dt = glm::min(time - lastTime, 1.0 / 10.0);
		lastTime = time;

		glfwPollEvents();

		editor->update(window, dt);
	}

	renderThreadStop = true;
	while (renderThreadStop);
	
	delete editor;

	glfwDestroyWindow(window);

	return 0;
}
