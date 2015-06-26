#pragma once
#include "ScriptComponent.h"

class LightMovementScript : public ScriptComponent
{
private:
	float time;
public:
	LightMovementScript();
	~LightMovementScript();

	void start();
	void update(float deltaTime);
};

