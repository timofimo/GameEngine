#pragma once

/*local includes*/
#include "GL/glew.h"
#include "GLFW/glfw3.h"

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
};

