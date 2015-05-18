#pragma once

class RenderingEngine;

/*local includes*/
#include "../GameComponent.h"

/*utility includes*/
#include <vector>

class RenderComponent : public GameComponent
{
private:
	GameObject* m_parent;
protected:
	std::vector<GameObject*> m_parents;
	RenderingEngine* m_renderingEngine;
public:
	RenderComponent(std::string name, RenderingEngine* renderingEngine) : GameComponent(GameComponent::RENDER, name)
	{
		m_renderingEngine = renderingEngine;
	}

	virtual void start(){}

	void addParent(GameObject* parent)
	{
		m_parents.push_back(parent);
	}
	virtual void removeParent(GameObject* parent) = 0;

private:
	void update(float deltaTime){}
	void setParent(GameObject* parent){ addParent(parent); }
};