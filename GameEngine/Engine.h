#pragma once

/*local includes*/
#include "Display.h"
#include "Shader.h"
#include "GameObject.h"

class Engine
{
private:
	Display m_mainDisplay;
	Shader m_shader;
	GameObject root;
public:
	Engine();
	~Engine();

private:
	void run();
	void update(float deltaTime);
	void render();
};

