#pragma once

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
public:
	RenderComponent(std::string name) : GameComponent(GameComponent::RENDER, name){}

	virtual void start(){}
	virtual void render(Shader* shader) = 0;

	void addParent(GameObject* parent)
	{
		m_parents.push_back(parent);
	}

private:
	void update(float deltaTime){}
	void setParent(GameObject* parent){ addParent(parent); }
};