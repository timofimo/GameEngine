#include "PointLight.h"

/*local includes*/
#include "../GameObject.h"
#include "../Shaders/InstancePointShader.h"

PointLight::PointLight(std::string name, glm::vec3 color, glm::vec3 position, float constant, float linear, float exponent, RenderingEngine* renderingEngine, LightComponent::LightTypes type) : LightComponent(name, color, 1.0f, renderingEngine, type)
{
	setAttenuation(glm::vec3(constant, linear, exponent));
	m_position = position;
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
	if (m_exponent == 0)
		return 0.0f;

	float colorMax = getColor().r > getColor().g ? (getColor().r > getColor().b ? getColor().r : getColor().b) : (getColor().g > getColor().b ? getColor().g : getColor().b);// highest of the three components
	float a = m_exponent;
	float b = m_linear;
	float c = m_constant - 256.0f * m_intensity * colorMax;
	return ((-b + glm::sqrt(b * b - 4 * a * c)) / (2 * a));
}

float PointLight::getRange(float lightAmount)
{
	if (lightAmount <= 0.0f || lightAmount > 1.0f)
		return getRange();
	if (m_exponent == 0)
		return 0.0f;

	float a = m_exponent;
	float b = m_linear;
	float m = 1 / lightAmount;
	float c = m_constant - m;
	return ((-b + glm::sqrt(b * b - 4 * a * c)) / (2 * a));
}

glm::vec3 PointLight::getPosition()
{
	Transform t = m_parent->getWorldTransform(false);
	glm::vec3 newPos = t.rotationQ() * m_position;
	return t.position() + newPos;
}

void PointLight::updateUniforms(Shader* shader)
{
	InstancePointShader* s = (InstancePointShader*)shader;
	s->setLightColor(getLight());
	s->setLightPosition(getPosition());
	s->setLightConstant(m_constant);
	s->setLightLinear(m_linear);
	s->setLightExponent(m_exponent);
}
