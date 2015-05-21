#include "Movement.h"

/*local includes*/
#include "../GameObject.h"

Movement::Movement() : ScriptComponent("Movement")
{
	m_passedTime = 0.0f;
}


Movement::~Movement()
{
}

void Movement::start()
{
	m_startPosition = m_parent->getLocalTransform().position();
}

void Movement::update(float deltaTime)
{
	m_passedTime += deltaTime;
	float translation = (glm::sin(m_passedTime) + 1.0f) * 30.0f;
	m_parent->getLocalTransform().setPosition(m_startPosition + glm::vec3(translation, 0.0f, 0.0f));
}
