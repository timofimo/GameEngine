#pragma once

/*local includes*/
#include "Display.h"
#include "Shader.h"
#include "GameObject.h"
#include "RenderingEngine.h"

class Engine
{
private:
	RenderingEngine renderingEngine;
	GameObject root;
public:
	Engine();
	~Engine();

private:
	void run();
	void update(float deltaTime);
	void render();
};

