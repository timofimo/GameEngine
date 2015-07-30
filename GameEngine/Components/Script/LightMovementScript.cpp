#include "LightMovementScript.h"

/*private includes*/
#include "../GameObjects/GameObject.h"

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
	m_parent->getLocalTransform().translate(glm::vec3(1.0f, 0.0f, 0.0f) * deltaTime);
}
