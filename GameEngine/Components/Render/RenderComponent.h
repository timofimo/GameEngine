#pragma once

/*local includes*/
#include "../GameComponent.h"

class RenderComponent : public GameComponent
{
public:
	RenderComponent(std::string name) : GameComponent(GameComponent::RENDER, name){}

	virtual void start(){}
	virtual void render(Shader* shader) = 0;

private:
	void update(float deltaTime){}
};