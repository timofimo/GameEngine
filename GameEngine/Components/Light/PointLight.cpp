#include "PointLight.h"


PointLight::PointLight(std::string name, glm::vec3 color, float constant, float linear, float exponent, RenderingEngine* renderingEngine) : LightComponent(name, color, 1.0f, renderingEngine)
{
	setAttenuation(glm::vec3(constant, linear, exponent));
}


PointLight::~PointLight()
{
}

float PointLight::getConstant()
{
	return m_constant;
}

void PointLight::setConstant(float constant)
{
	m_constant = constant;
}

float PointLight::getLinear()
{
	return m_linear;
}

void PointLight::setLinear(float linear)
{
	m_linear = linear;
}

float PointLight::getExponent()
{
	return m_constant;
}

void PointLight::setExponent(float exponent)
{
	m_exponent = exponent;
}

glm::vec3 PointLight::getAttenuation()
{
	return glm::vec3(m_constant, m_linear, m_exponent);
}

void PointLight::setAttenuation(glm::vec3 attenuation)
{
	setConstant(attenuation.x);
	setLinear(attenuation.y);
	setExponent(attenuation.z);
}

float PointLight::getRange()
{
	float colorMax = getColor().r > getColor().g ? (getColor().r > getColor().b ? getColor().r : getColor().b) : (getColor().g > getColor().b ? getColor().g : getColor().b);// highest of the three components
	float a = m_exponent;
	if (a == 0)
		a = 0.0001f;
	float b = m_linear;
	float c = m_constant - 1.0f * m_intensity * colorMax;
	return ((-b + glm::sqrt(b * b - 4 * a * c)) / (2 * a));
}
