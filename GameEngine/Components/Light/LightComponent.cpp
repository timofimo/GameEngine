#include "LightComponent.h"

/*local includes*/
#include "../RenderingEngine.h"

LightComponent::LightComponent(std::string name, glm::vec3 color, float intensity, RenderingEngine* renderingEngine) : GameComponent(GameComponent::LIGHT, name)
{
	setColor(color);
	setIntensity(intensity);
	m_renderingEngine = renderingEngine;
	m_renderingEngine->addLight(this);
}


LightComponent::~LightComponent()
{
	m_renderingEngine->removeLight(this);
}

glm::vec3 LightComponent::getColor()
{
	return m_color;
}

void LightComponent::setColor(glm::vec3 color)
{
	m_color = glm::clamp(color, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(256.0f, 256.0f, 256.0f));
}

float LightComponent::getIntensity()
{
	return m_intensity;
}

void LightComponent::setIntensity(float intensity)
{
	m_intensity = intensity;
}

glm::vec3 LightComponent::getLight()
{
	return m_color * m_intensity;
}