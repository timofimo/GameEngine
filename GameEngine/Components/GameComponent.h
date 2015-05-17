#pragma once

class GameObject;
class Shader;

/*local includes*/
#include "../Transform.h"

/*utility includes*/
#include <iostream>

class GameComponent
{
public:
	enum ComponentType
	{
		SCRIPT,
		RENDER
	};
protected:
	const ComponentType m_type;
	const std::string m_name;
	GameObject* m_parent;

public:
	GameComponent(ComponentType type, std::string name) : m_type(type), m_name(name){}
	virtual ~GameComponent(){}

	virtual void start() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render(Shader* shader) = 0;

	ComponentType getComponentType(){ return m_type; }
	std::string getName(){ return m_name; }

	virtual void setParent(GameObject* parent){ this->m_parent = parent; }
};