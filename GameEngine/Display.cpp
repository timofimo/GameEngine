#include "Display.h"

unsigned int Display::m_width;
unsigned int Display::m_height;
GLFWwindow* Display::m_window;


Display::Display(unsigned int width, unsigned int height, const std::string name, const bool fullscreen/* = false*/, const bool useScreenResolution/* = false*/)
{
	// start GL context using GLFW
	if (!glfwInit())
	{
		std::cerr << "ERROR: could not start GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);// the window can not be resized

	if (fullscreen)
	{
		if (useScreenResolution)
		{
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			m_window = glfwCreateWindow(mode->width, mode->height, name.c_str(), glfwGetPrimaryMonitor(), NULL);
			width = mode->width;
			height = mode->height;
		}
		m_window = glfwCreateWindow(width, height, name.c_str(), glfwGetPrimaryMonitor(), NULL);
	}
	else
		m_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (!m_window)
	{
		std::cerr << "ERROR: could not open window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// set class variables
	m_width = width;
	m_height = height;

	// set the current GL context
	glfwMakeContextCurrent(m_window);

	// start GLEW
	glewExperimental = GL_TRUE;
	glewInit();

#ifdef _DEBUG
	// get version info
	const GLubyte* render = glGetString(GL_RENDERER); // get the renderer
	const GLubyte* version = glGetString(GL_VERSION); // get the GL version
	std::cout << "Renderer: " << render << std::endl;
	std::cout << "OpenGL version supported " << version << std::endl;
#endif

	glfwSetWindowPos(m_window, 500, 450);

	// enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); // a smaller value is closer
	// enable back face culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	// disable vSync
	glfwSwapInterval(0);

	// set GLFW callbacks
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetCursorPosCallback(m_window, cursor_pos_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetCursorEnterCallback(m_window, cursor_enter_callback);
	Input::setParentWindow(m_window);
}


Display::~Display()
{
	glfwTerminate();
}

int Display::shouldClose()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(m_window, 1);
	return glfwWindowShouldClose(m_window);
}

unsigned int Display::getWidth()
{
	return m_width;
}

unsigned int Display::getHeight()
{
	return m_height;
}

GLFWwindow* Display::getWindow()
{
	return m_window;
}

void Display::error_callback(int error, const char* description)
{
	std::cout << "GLFW ERROR: " << description << std::endl;
}

void Display::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	Input::setKey(key, action);
}

void Display::cursor_pos_callback(GLFWwindow* window, double x, double y)
{
	Input::setMousePos(x, y);
}

void Display::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Input::setMouseButton(button, action);
}

void Display::cursor_enter_callback(GLFWwindow* window, int entered)
{
	Input::setCursorWithinBounds(entered);
}
