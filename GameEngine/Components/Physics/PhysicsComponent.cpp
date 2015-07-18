#include "PhysicsComponent.h"


PhysicsComponent::PhysicsComponent(PhysicsComponentType type, std::string name) : GameComponent(GameComponent::PHYSICS, name)
{
	m_type = type;
}


PhysicsComponent::~PhysicsComponent()
{
}

PhysicsComponent::PhysicsComponentType PhysicsComponent::getType()
{
	return m_type;
}
