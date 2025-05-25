#include <GL/glew.h>
#include <Editor.h>

Editor::Editor(int argc, const char* argv[])
{

}

void Editor::initRender(GLFWwindow* window)
{
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Editor::updateSize(GLFWwindow* window)
{
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);

	glViewport(0, 0, w, h);
}
void Editor::update(GLFWwindow* window, double dt)
{

}
void Editor::render(GLFWwindow* window, double dt)
{
	static double time = 0.0;
	time += dt;
	glm::vec4 a = glm::vec4{ cos(time), sin(time), cos(sin(time)), 1.0 } * 0.5f + 0.5f;
	glClearNamedFramebufferfv(0, GL_COLOR, 0, &a[0]);
}

void Editor::fileDropCallback(GLFWwindow* window, int count, const char* paths[])
{

}
void Editor::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}
void Editor::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{

}
void Editor::scrollCallback(GLFWwindow* window, double xoff, double yoff)
{

}
void Editor::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);
}
void Editor::charCallback(GLFWwindow* window, uint32_t codepoint)
{

}

Editor::~Editor()
{

}
