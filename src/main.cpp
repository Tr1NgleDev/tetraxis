#include <basicIncludes.h>

#include <Editor.h>
#include <stb_image.h>
#include <resvg.h>

#ifdef _WIN32
extern "C"
{
	_declspec(dllexport) int NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

GLFWwindow* window = nullptr;
Editor* editor = nullptr;

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
	editor->updateSize(window);
	editor->render(window, 0.0);
	glfwSwapBuffers(window);
}
void fileDropCallback(GLFWwindow* window, int count, const char* paths[])
{
	editor->fileDropCallback(window, count, paths);
}

#ifndef NDEBUG
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
	GLsizei length, const char* message, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	printf("[GL Debug]: ");

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:				printf("[Error]: "); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	printf("[Deprecated Behaviour]: "); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	printf("[Undefined Behaviour]: "); break;
	case GL_DEBUG_TYPE_PORTABILITY:			printf("[Portability]: "); break;
	case GL_DEBUG_TYPE_PERFORMANCE:			printf("[Performance]: "); break;
	case GL_DEBUG_TYPE_MARKER:				printf("[Marker]: "); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:			printf("[Push Group]: "); break;
	case GL_DEBUG_TYPE_POP_GROUP:			printf("[Pop Group]: "); break;
	case GL_DEBUG_TYPE_OTHER:				printf("[Other]: "); break;
	}

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             printf("[API]: "); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   printf("[Window System]: "); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: printf("[Shader Compiler]: "); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     printf("[Third Party]: "); break;
	case GL_DEBUG_SOURCE_APPLICATION:     printf("[Application]: "); break;
	case GL_DEBUG_SOURCE_OTHER:           printf("[Other]: "); break;
	}
	printf("\n(%i): %s\n", id, message);
}
#endif

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

#ifndef NDEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

	window = glfwCreateWindow(1280, 720, "Tetraxis", nullptr, nullptr);
	if (!window)
	{
		printf("Failed to initialize the window!\n");
		return 2;
	}

	// load the window icon
	//{
	//	std::vector<GLFWimage> imgs{};
	//
	//	resvg_options* opt = resvg_options_create();
	//	resvg_options_load_system_fonts(opt);
	//
	//	resvg_render_tree* svg;
	//	int err = resvg_parse_tree_from_file("assets/icon/icon.svg", opt, &svg);
	//	if (!err)
	//	{
	//		constexpr int sizes[]
	//		{
	//			16,
	//			32,
	//			64,
	//			128,
	//			512
	//		};
	//		constexpr size_t count = sizeof(sizes) / sizeof(sizes[0]);
	//
	//		imgs.reserve(count);
	//		for (int i = 0; i < count; ++i)
	//		{
	//			int s = sizes[i];
	//			uint8_t* data = new uint8_t[s * s * 4]{ 0 };
	//			if (data)
	//			{
	//				resvg_size size = resvg_get_image_size(svg);
	//
	//				auto transform = resvg_transform_identity();
	//				transform.a = s / size.width;
	//				transform.d = s / size.height;
	//				//transform.f += height;
	//
	//				resvg_render(svg, transform, s, s, (char*)data);
	//				imgs.push_back(GLFWimage{ s, s, data });
	//			}
	//		}
	//	}
	//	resvg_tree_destroy(svg);
	//	resvg_options_destroy(opt);
	//
	//	if (!imgs.empty())
	//		glfwSetWindowIcon(window, imgs.size(), imgs.data());
	//
	//	for (auto& img : imgs)
	//		delete[] img.pixels;
	//}
	// load the window icon (i do NOT want to figure out how to take them out of the .ico embedded into the .exe resources... yet.)
	{
		std::vector<GLFWimage> imgs{};

		constexpr const char* icons[3]
		{
			"assets/icon/icon_32.png",
			"assets/icon/icon_64.png",
			"assets/icon/icon_512.png"
		};
		imgs.reserve(3);
		for (int i = 0; i < 3; ++i)
		{
			int w, h, channels;
			uint8_t* data = stbi_load(icons[i], &w, &h, &channels, 4);
			if (data)
			{
				imgs.push_back(GLFWimage{ w, h, data });
			}
		}

		if (!imgs.empty())
			glfwSetWindowIcon(window, imgs.size(), imgs.data());

		for (auto& img : imgs)
			stbi_image_free(img.pixels);
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

	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit())
	{
		printf("Failed to initialize GLEW!\n");
		glfwSetWindowShouldClose(window, true);
		return 3;
	}

#ifndef NDEBUG
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
#endif


	glfwSwapInterval(1);

	QuadRenderer::init();

	editor = new Editor(argc, argv);
	glfwSetWindowUserPointer(window, editor);

	editor->init(window);
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

	delete editor;

	Texture::destroy();
	Shader::destroy();
	QuadRenderer::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
