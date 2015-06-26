#include "ParentScript.h"

/*private includes*/
#include "../../GameObject.h"

ParentScript::ParentScript() : ScriptComponent("ParentScript")
{
}


ParentScript::~ParentScript()
{
}

void ParentScript::update(float deltaTime)
{
	/*m_time += deltaTime;
	float amount = 5.0f;
	if (Input::getKey(GLFW_KEY_SPACE))
		amount = 45.0f;
	m_parent->getLocalTransform().rotate(glm::radians(glm::vec3(0.0f, amount, 0.0f) * deltaTime));
	m_parent->getLocalTransform().setPosition(glm::vec3(0.0f, glm::sin(m_time) + 1.0f, 0.0f));*/
	m_parent->getLocalTransform().translate(glm::vec3(0.1f, 0.0f, 0.0f) * deltaTime);
	m_parent->getLocalTransform().printPosition();
}
