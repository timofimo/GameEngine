#include "LightComponent.h"

/*local includes*/
#include "../RenderingEngine.h"
#include "../GameObjects/GameObject.h"

LightComponent::LightComponent(std::string name, LightType type) : GameComponent(GameComponent::LIGHT, name)
{
	m_type = type;
	RenderingEngine::get().addLight(this);
}


LightComponent::~LightComponent()
{
	RenderingEngine::get().removeLight(this);
}

void LightComponent::setIntensity(float intensity)
{
	m_intensity = intensity;
}

float LightComponent::getIntensity()
{
	return m_intensity;
}

void LightComponent::setColor(glm::vec3 color)
{
	m_color = color;
}

glm::vec3 LightComponent::getColor()
{
	return m_color;
}

glm::vec3 LightComponent::getLight()
{
	return m_color * m_intensity;
}

LightType LightComponent::getType()
{
	return m_type;
}

void LightComponent::enableShadowCasting(bool softShadows /*= false*/)
{
	m_castsShadows = true;
	m_castsSoftShadows = softShadows;
}

void LightComponent::disableShadowCasting()
{
	m_castsShadows = false;
}

void LightComponent::setShadowMapIndex(int index)
{
	m_shadowMapIndex = index;
}

int LightComponent::getShadowMapIndex()
{
	return m_shadowMapIndex;
}

bool LightComponent::castsShadows()
{
	return m_castsShadows;
}

bool LightComponent::castsSoftShadows()
{
	return m_castsSoftShadows;
}

glm::vec3 LightComponent::getPosition()
{
	return m_parent->getWorldTransform(false).position();
}

glm::vec3 LightComponent::getDirection()
{
	return m_parent->getWorldTransform(false).forward();
}

float LightComponent::getRange(float lightAmount)
{
	return FLT_MAX;
}
