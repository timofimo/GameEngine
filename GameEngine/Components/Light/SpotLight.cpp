#include "SpotLight.h"

/*local includes*/
#include "../GameObject.h"
#include "../Shaders/InstanceSpotShader.h"

SpotLight::SpotLight(std::string name, glm::vec3 color, glm::vec3 position, glm::vec3 direction, float angle, float constant, float linear, float exponent, RenderingEngine* renderingEngine) : PointLight(name, color, position, constant, linear, exponent, renderingEngine, LightComponent::SPOT_LIGHT)
{
	setDirection(direction);
	setAngle(angle);
}


SpotLight::~SpotLight()
{
}

glm::vec3 SpotLight::getDirection()
{
	return m_direction;
}

void SpotLight::setDirection(glm::vec3 direction)
{
	m_direction = glm::normalize(direction);
}

float SpotLight::getAngle()
{
	return m_angle;
}

void SpotLight::setAngle(float angle)
{
	m_angle = angle;
}

void SpotLight::updateUniforms(Shader* shader)
{
	InstanceSpotShader* s = (InstanceSpotShader*)shader;
	s->setLightColor(getLight());
	s->setLightPosition(getPosition());
	s->setLightConstant(m_constant);
	s->setLightLinear(m_linear);
	s->setLightExponent(m_exponent);
	s->setLightCutOff(m_angle);
	s->setLightDirection(m_direction);
}
