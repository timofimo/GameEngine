#pragma once

/*local includes*/
#include "GameObject.h"
#include "Shader.h"
#include "Components/Script/Camera.h"

class RenderingEngine
{
private:
	Shader* activeShader;
	Camera* activeCamera;
public:
	RenderingEngine();
	~RenderingEngine();

	void render(GameObject* rootObject);
};

