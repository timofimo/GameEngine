#include "SphereComponent.h"

/*local includes*/
#include "FrustumComponent.h"
#include "../GameObject.h"

SphereComponent::SphereComponent(glm::vec3 origin, float range) : PhysicsComponent(PhysicsComponent::SPHERE_COMPONENT, "sphereComponent")
{
	m_origin = origin;
	m_range = range;
}

SphereComponent::~SphereComponent()
{
}

void SphereComponent::setOrigin(glm::vec3 origin)
{
	m_origin = origin;
}

glm::vec3 SphereComponent::getOrigin(glm::vec3 offset/* = glm::vec3()*/)
{
	glm::vec3 result = m_origin + offset;
	if (m_parent != nullptr)
	{
		result += m_parent->getWorldTransform(false).position();
	}
	return result;
}

void SphereComponent::setRange(float range)
{
	m_range = range;
}

float SphereComponent::getRange()
{
	return m_range;
}

void SphereComponent::setSphere(glm::vec3 origin, float range)
{
	m_origin = origin;
	m_range = range;
}

bool SphereComponent::checkCollision(PhysicsComponent* component)
{
	switch (component->getType())
	{
	case PhysicsComponentType::FRUSTUM_COMPONENT:
		((FrustumComponent*)component)->checkCollision(this);
	default:
		break;
	}
	return true;
}
