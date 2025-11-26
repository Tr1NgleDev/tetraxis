#include <Editor.h>

Editor::Editor(int argc, const char* argv[])
{

}

void Editor::init(GLFWwindow* window)
{
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader::load("postShader", {
		{ GL_VERTEX_SHADER, "assets/shaders/post.vert" },
		{ GL_FRAGMENT_SHADER, "assets/shaders/post.frag" } });
	Shader::load("quadShader", {
		{ GL_VERTEX_SHADER, "assets/shaders/quad.vert" },
		{ GL_FRAGMENT_SHADER, "assets/shaders/quad.frag" } });
	Shader::load("meshShader", {
		{ GL_VERTEX_SHADER, "assets/shaders/mesh.vert" },
		{ GL_GEOMETRY_SHADER, "assets/shaders/mesh.geom" },
		{ GL_FRAGMENT_SHADER, "assets/shaders/mesh.frag" } });
	Texture::load("assets/icon/icon_512.png", 0, false, "icon");

	fb.shader = Shader::get("postShader");
	meshShader = Shader::get("meshShader");

	mesh = fdo::Object::load4DOFromFile("./assets/models/slingshot.4do");
	meshRenderer = &mesh;
}

void Editor::updateSize(GLFWwindow* window)
{
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);

	if (!w || !h) return;

	glViewport(0, 0, w, h);

	fb.init(w, h,
		{
			{"tex", { GL_COLOR_ATTACHMENT0, GL_RGBA8 }},
			{"depth", { GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24 }},
		}, 4);

	projection2D = glm::ortho(0.0f, (float)w, (float)h, 0.0f, -1.0f, 1.0f);
	projection3D = glm::perspective(glm::radians(90.0f), (float)w / (float)h, 0.01f, 100.0f);
	
	Shader::get("quadShader")->setUniform("proj", projection2D);
	meshShader->setUniform("P", projection3D);
}
void Editor::update(GLFWwindow* window, double dt)
{
}
void Editor::render(GLFWwindow* window, double dt)
{
	const glm::vec4 clearColor{ 0 };
	const float clearDepth = 1.0f;

	fb.useDefault();
	glClearNamedFramebufferfv(0, GL_COLOR, 0, &clearColor[0]);
	glClearNamedFramebufferfv(0, GL_DEPTH, 0, &clearDepth);

	fb.use();
	{
		glClearNamedFramebufferfv(fb.id(), GL_COLOR, 0, &clearColor[0]);
		glClearNamedFramebufferfv(fb.id(), GL_DEPTH, 0, &clearDepth);
		glEnable(GL_DEPTH_TEST);

		m4::Mat5 MV = m4::createCamera({ 0,0,6,0.001f }, { -1,0,0,0 }, { 0,1,0,0 }, { 0,0,-1,0 }, { 0,0,0,1 });
		m4::Rotor rot =
			m4::Rotor{ m4::BiVector4::XZ(), (float)glm::radians(glfwGetTime() * 45.0f) } *
			m4::Rotor{ m4::BiVector4::YZ(), (float)glm::radians(glfwGetTime() * 45.0f) } *
			m4::Rotor{ m4::BiVector4::YW(), (float)glm::radians(glfwGetTime() * 45.0f) };
		MV *= rot;
		MV.translate(0, -3, 0, 0);
		meshShader->use();
		meshShader->setUniform("MV", MV);
		meshRenderer.render();

		glDisable(GL_DEPTH_TEST);
	}

	fb.useDefault();
	fb.render();
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
		fb.shader->reload();
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);

	if (action == GLFW_REPEAT) return;
}
void Editor::charCallback(GLFWwindow* window, uint32_t codepoint)
{

}

Editor::~Editor()
{
}
