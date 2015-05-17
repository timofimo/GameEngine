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
	m_parent->getLocalTransform().rotate(glm::radians(glm::vec3(0.0f, 5.0f, 0.0f) * deltaTime));
}
