#include "LightMovementScript.h"

/*private includes*/
#include "../../GameObject.h"

LightMovementScript::LightMovementScript() : ScriptComponent("LightMovement")
{
}


LightMovementScript::~LightMovementScript()
{
}

void LightMovementScript::start()
{

}

void LightMovementScript::update(float deltaTime)
{
	time += deltaTime;
	Transform t = m_parent->getLocalTransform();
	m_parent->getLocalTransform().setPosition(glm::vec3(t.position().x, glm::sin(time) * 2.5f + 3.5f, t.position().z));
}
