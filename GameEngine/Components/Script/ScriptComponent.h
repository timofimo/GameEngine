#pragma once

/*local includes*/
#include "../GameComponent.h"

class ScriptComponent : public GameComponent
{
public:
	ScriptComponent(std::string name) : GameComponent(GameComponent::SCRIPT, name){}

	virtual void start(){}
	virtual void update(float deltaTime) = 0;
};