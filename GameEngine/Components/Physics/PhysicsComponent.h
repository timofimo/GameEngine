#pragma once

/*local includes*/
#include "../GameComponent.h"

class PhysicsComponent : public GameComponent
{
protected:
	enum PhysicsComponentType
	{
		FRUSTUM_COMPONENT, 
		SPHERE_COMPONENT,
	}; PhysicsComponentType m_type;
public:
	PhysicsComponent(PhysicsComponentType type, std::string name);
	~PhysicsComponent();

	virtual bool checkCollision(PhysicsComponent* component) = 0;
	PhysicsComponentType getType();
private:
	void update(float deltaTime){}
};

