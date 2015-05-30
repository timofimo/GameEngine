#pragma once

/*local includes*/
#include "GL/glew.h" // error gl.h before glew.h
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

class Input
{
	friend class Display;
private:
	static GLFWwindow* m_parentWindow;
	enum KeyState
	{
		UP,
		PRESS,
		HOLD,
		RELEASE
	};

	static KeyState m_ks[GLFW_KEY_LAST];
	static KeyState m_mbs[GLFW_MOUSE_BUTTON_LAST];
	static glm::vec2 m_mousePos, m_mouseLastPos;
	static bool m_cursorWithinBounds;
public:
	Input();
	~Input();

	static void initialize();
	static void reset();
	static bool getKey(int key);
	static bool getKeyDown(int key);
	static bool getKeyUp(int key);

	static bool getMouseButton(int button);
	static bool getMouseButtonDown(int button);
	static bool getMouseButtonUp(int button);

	static glm::vec2 getMousePosition();
	static glm::vec2 getMouseTranslation();
	static void setMousePosition(glm::vec2 pos);
	static void setMouseMode(int mode);

	static bool cursorWithinBounds();

private:
	static void setKey(int key, int action);
	static void setMouseButton(int button, int action);
	static void setMousePos(double x, double y);
	static void setCursorWithinBounds(int action);
	static void setParentWindow(GLFWwindow* window);
};

