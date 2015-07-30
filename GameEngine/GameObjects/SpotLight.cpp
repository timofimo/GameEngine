#include "SpotLight.h"


SpotLight::SpotLight() : GameObject("SpotLight")
{
	m_spotLight = new SpotLightComponent();
	this->addComponent(m_spotLight);
	m_spotLight->setParent(this);
}

SpotLight::SpotLight(glm::vec3 color, float intensity, float cutoff, float exponent) : SpotLight()
{
	setLightParameters(color, intensity, cutoff, exponent);
}

SpotLight::SpotLight(glm::vec3 color, float intensity, float cutoff, float exponent, glm::vec3 position, glm::vec3 rotation) : SpotLight(color, intensity, cutoff, exponent)
{
	getLocalTransform().setPosition(position)->setRotation(rotation);
}

SpotLight::SpotLight(glm::vec3 color, float intensity, float cutoff, float exponent, glm::vec3 position, glm::vec3 rotation, bool castsSoftShadows) : SpotLight(color, intensity, cutoff, exponent, position, rotation)
{
	setShadowParameters(true, castsSoftShadows);
}


SpotLight::~SpotLight()
{
}

void SpotLight::setLightParameters(glm::vec3 color, float intensity, float cutoff, float exponent)
{
	m_spotLight->setColor(color);
	m_spotLight->setIntensity(intensity);
	m_spotLight->setCutoff(cutoff);
	m_spotLight->setExponent(exponent);
}

void SpotLight::setShadowParameters(bool castsShadows, bool castsSoftShadows)
{
	if (castsShadows)
		m_spotLight->enableShadowCasting(castsSoftShadows);
	else
		m_spotLight->disableShadowCasting();
}
