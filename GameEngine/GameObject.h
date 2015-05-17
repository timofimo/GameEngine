#pragma once

/*local includes*/
#include "Transform.h"
#include "Components/GameComponent.h"
#include "Shader.h"

/*utility includes*/
#include <vector>
#include <iostream>

class GameObject
{
private:
	std::string m_name;
	GameObject* m_parent;
	std::vector<GameObject*> m_children;
	std::vector<GameComponent*> m_components;
	Transform m_localTransform, m_worldTransform;
public:
	GameObject(const std::string name);
	~GameObject();

/*	bool operator==(GameObject other);
	GameObject& operator=(const GameObject& other);*/

	void start();
	void update(float deltaTime);
	void render(Shader* shader);

	void setParent(GameObject* parent);

	void addChild(GameObject* child);
	void removeChild(const std::string name);

	void addComponent(GameComponent* component);
	void removeComponent(const std::string name);

	Transform& getLocalTransform();
	Transform& getWorldTransform();
	bool transformChanged();

	std::string getName();
};

