#include <GL/glew.h>
#include <Editor.h>
//#include <qpainter.h>
//#include <qimage.h>

Editor::Editor(int argc, const char* argv[])
{

}

void Editor::initRender(GLFWwindow* window)
{
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	bool a = Shader::load("testShader", { { GL_VERTEX_SHADER, "assets/quad.vert" }, { GL_FRAGMENT_SHADER, "assets/quad.frag" }});
	testShader = Shader::get("testShader");

	QuadRenderer::init();

	Texture::load("assets/icon.png");
	tex = Texture::get("assets/icon.png");

	//svgRenderer.load(QString("assets/icon.svg"));
}

void Editor::updateSize(GLFWwindow* window)
{
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);

	glViewport(0, 0, w, h);

	//float a = (float)w / (float)h;
	//int aW = 64 * a;
	//int aH = 64;
	//QImage image{ aW, aH, QImage::Format_RGBA8888 };
	//image.fill(0x00000000);
	//QPainter painter{ &image };
	//svgRenderer.render(&painter, image.rect());
	//image.mirror();
	//svgTex.init(image.constBits(), Texture::Format{ GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE }, aW, aH, true);
}
void Editor::update(GLFWwindow* window, double dt)
{

}
void Editor::render(GLFWwindow* window, double dt)
{
	glm::vec4 clearColor{ 0 };
	float clearDepth = 1;

	glClearNamedFramebufferfv(0, GL_COLOR, 0, &clearColor[0]);
	glClearNamedFramebufferfv(0, GL_DEPTH, 0, &clearDepth);

	testShader->use();
	testShader->setUniform("tex", 0);
	tex->use();
	//svgTex.use();
	QuadRenderer::render();
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
	if (action == GLFW_PRESS && key == GLFW_KEY_R)
		testShader->reload();
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);
}
void Editor::charCallback(GLFWwindow* window, uint32_t codepoint)
{

}

Editor::~Editor()
{
	Texture::destroy();
	Shader::destroy();
	QuadRenderer::destroy();
}
