#include "SpotLight.h"


SpotLight::SpotLight(std::string name, glm::vec3 color, glm::vec3 direction, float angle, float constant, float linear, float exponent, RenderingEngine* renderingEngine) : PointLight(name, color, constant, linear, exponent, renderingEngine)
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
