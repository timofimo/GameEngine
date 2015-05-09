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
	m_parent->getWorldTransform().printPosition();
}
