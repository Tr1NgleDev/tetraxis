#include <GL/glew.h>
#include <basicIncludes.h>

#include <Editor.h>

#ifdef _WIN32
extern "C"
{
	_declspec(dllexport) int NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

GLFWwindow* window = nullptr;
Editor* editor = nullptr;
//std::atomic<bool> renderWindowResized = true;

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
	//renderWindowResized = true;
	editor->updateSize(window);
	editor->render(window, 0.0);
	glfwSwapBuffers(window);
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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//#ifdef __APPLE__ // i would like to use gl 4.5 for this
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
//#endif
	const char* sessionType = std::getenv("XDG_SESSION_TYPE");
	if (sessionType && std::string(sessionType) == "wayland")
		glfwWindowHintString(GLFW_WAYLAND_APP_ID, "dev.tr1ngle.tetraxis");

	window = glfwCreateWindow(1280, 720, "Tetraxis", nullptr, nullptr);
	if (!window)
	{
		printf("Failed to initialize the window!\n");
		return 2;
	}

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

	/*std::thread renderThread(
		[&argc, &argv]() -> void
		{
			glfwMakeContextCurrent(window);
			glewExperimental = true;
			if (glewInit())
			{
				printf("Failed to initialize GLEW!\n");
				glfwSetWindowShouldClose(window, true);
				return;
			}
			
			glfwSwapInterval(1);

			editor->initRender(window);
			editor->updateSize(window);

			double lastTime = glfwGetTime() - 0.01;
			while (true)
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

				if (glfwWindowShouldClose(window))
					break;
			}
		});
	renderThread.detach();*/

	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit())
	{
		printf("Failed to initialize GLEW!\n");
		glfwSetWindowShouldClose(window, true);
		return 3;
	}

	glfwSwapInterval(1);

	editor->initRender(window);
	editor->updateSize(window);

	double lastTime = glfwGetTime() - 0.01;
	while (true)
	{
		double time = glfwGetTime();
		double dt = glm::min(time - lastTime, 1.0 / 10.0);
		lastTime = time;

		glfwPollEvents();

		editor->update(window, dt);

		editor->render(window, dt);

		glfwSwapBuffers(window);

		if (glfwWindowShouldClose(window))
			break;
	}

	//if (renderThread.joinable())
	//	renderThread.join();
	
	delete editor;

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
