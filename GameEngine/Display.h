#pragma once

/*local includes*/
#include "GL/glew.h"
#include "Input.h"

/*utility includes*/
#include <iostream>

class Display
{
private:
	static unsigned int m_width, m_height;
	static GLFWwindow* m_window;

public:
	Display(unsigned int width, unsigned int height, const std::string name, const bool fullscreen = false, const bool useScreenResolution = false);
	~Display();

	int shouldClose();
	static unsigned int getWidth();
	static unsigned int getHeight();
	static GLFWwindow* getWindow();

private:
	static void error_callback(int error, const char* description);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_pos_callback(GLFWwindow* window, double x, double y);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_enter_callback(GLFWwindow* window, int entered);
};

