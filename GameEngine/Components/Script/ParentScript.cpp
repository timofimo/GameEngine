#include "ParentScript.h"

/*private includes*/
#include "../GameObjects/GameObject.h"

ParentScript::ParentScript() : ScriptComponent("ParentScript")
{
}


ParentScript::~ParentScript()
{
}

void ParentScript::update(float deltaTime)
{
	m_time += deltaTime;
	float amount = 5.0f;
	if (Input::getKey(GLFW_KEY_SPACE))
		amount = 45.0f;
	m_parent->getLocalTransform().rotate(glm::radians(glm::vec3(0.0f, amount, 0.0f) * deltaTime));
}
