#include "Input.h"

GLFWwindow* Input::m_parentWindow;
Input::KeyState Input::m_ks[];
Input::KeyState Input::m_mbs[];
glm::vec2 Input::m_mousePos;
glm::vec2 Input::m_mouseLastPos;
bool Input::m_cursorWithinBounds;

/*utility includes*/
#include <iostream>


Input::Input()
{
}


Input::~Input()
{
}

void Input::initialize()
{
	double x, y;
	glfwGetCursorPos(m_parentWindow, &x, &y);
	setMousePosition(glm::vec2(x, y));
	reset();
}

void Input::reset()
{
	for (int keyIndex = 0; keyIndex < GLFW_KEY_LAST; keyIndex++)
	{
		if (m_ks[keyIndex] == PRESS)
			m_ks[keyIndex] = HOLD;
		else if (m_ks[keyIndex] == RELEASE)
			m_ks[keyIndex] = UP;
	}
	for (int buttonIndex = 0; buttonIndex < GLFW_MOUSE_BUTTON_LAST; buttonIndex++)
	{
		if (m_mbs[buttonIndex] == PRESS)
			m_mbs[buttonIndex] = HOLD;
		else if (m_mbs[buttonIndex] == RELEASE)
			m_mbs[buttonIndex] = UP;
	}

	m_mouseLastPos = m_mousePos;
}

bool Input::getKey(int key)
{
	return m_ks[key] == PRESS || m_ks[key] == HOLD;
}

bool Input::getKeyDown(int key)
{
	return m_ks[key] == PRESS;
}

bool Input::getKeyUp(int key)
{
	return m_ks[key] == RELEASE;
}

bool Input::getMouseButton(int button)
{
	return m_mbs[button] == PRESS || m_mbs[button] == HOLD;
}

bool Input::getMouseButtonDown(int button)
{
	return m_mbs[button] == PRESS;
}

bool Input::getMouseButtonUp(int button)
{
	return m_mbs[button] == RELEASE;
}

glm::vec2 Input::getMousePosition()
{
	return m_mousePos;
}

glm::vec2 Input::getMouseTranslation()
{
	return m_mouseLastPos - m_mousePos;
}

void Input::setMousePosition(glm::vec2 pos)
{
	if (m_parentWindow)
	{
		glfwSetCursorPos(m_parentWindow, pos.x, pos.y);
		m_mousePos = m_mouseLastPos = pos;
	}
}

void Input::setMouseMode(int mode)
{
	if (mode < GLFW_CURSOR_NORMAL || mode > GLFW_CURSOR_DISABLED)
		return;

	glfwSetInputMode(m_parentWindow, GLFW_CURSOR, mode);
}

bool Input::cursorWithinBounds()
{
	return m_cursorWithinBounds;
}

void Input::setKey(int key, int action)
{
	if (action == GLFW_REPEAT)
		return;

	if (action == GLFW_PRESS)
	{
		if (m_ks[key] == UP)
			m_ks[key] = PRESS;
		else if (m_ks[key] == PRESS)
			m_ks[key] = HOLD;
	}
	else if (action == GLFW_RELEASE)
	{
		if (m_ks[key] == PRESS || m_ks[key] == HOLD)
			m_ks[key] = RELEASE;
	}
	else
	{
		m_ks[key] = UP;
	}
}

void Input::setMouseButton(int button, int action)
{
	if (action == GLFW_REPEAT)
		return;

	if (action == GLFW_PRESS)
	{
		if (m_mbs[button] == UP)
			m_mbs[button] = PRESS;
		else if (m_mbs[button] == PRESS)
			m_mbs[button] = HOLD;
	}
	else if (action == GLFW_RELEASE)
	{
		if (m_mbs[button] == PRESS || m_mbs[button] == HOLD)
			m_mbs[button] = RELEASE;
	}
	else
	{
		m_mbs[button] = UP;
	}
}

void Input::setMousePos(double x, double y)
{
	m_mousePos = glm::vec2(x, y);
}

void Input::setCursorWithinBounds(int action)
{
	if (action == GL_TRUE)
		m_cursorWithinBounds = true;
	else
		m_cursorWithinBounds = false;
}

void Input::setParentWindow(GLFWwindow* window)
{
	m_parentWindow = window;
}