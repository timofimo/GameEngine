#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : GameObject("DirectionalLight")
{
	m_directionalLight = new DirectionalLightComponent();
	this->addComponent(m_directionalLight);
	m_directionalLight->setParent(this);
}

DirectionalLight::DirectionalLight(glm::vec3 color, float intensity) : DirectionalLight()
{
	setLightParameters(color, intensity);
}

DirectionalLight::DirectionalLight(glm::vec3 color, float intensity, bool castsSoftShadows, float shadowSize, float shadowZNear, float shadowZFar) : DirectionalLight(color, intensity)
{
	setShadowParameters(true, castsSoftShadows, shadowSize, shadowZNear, shadowZFar);
}


DirectionalLight::~DirectionalLight()
{
	
}

void DirectionalLight::setLightParameters(glm::vec3 color, float intensity)
{
	m_directionalLight->setColor(color);
	m_directionalLight->setIntensity(intensity);
}

void DirectionalLight::setShadowParameters(bool castsShadows, bool castsSoftShadows, float shadowSize, float shadowZNear, float shadowZFar)
{
	if (castsShadows)
		m_directionalLight->enableShadowCasting(castsSoftShadows, shadowSize, shadowZNear, shadowZFar);
	else
		m_directionalLight->disableShadowCasting();
}
