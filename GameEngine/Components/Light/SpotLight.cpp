#include "SpotLight.h"

/*local includes*/
#include "../GameObject.h"
#include "../RenderingEngine.h"

SpotLight::SpotLight(std::string name, glm::vec3 color, glm::vec3 position, glm::vec3 direction, float angle, float constant, float linear, float exponent, RenderingEngine* renderingEngine) : PointLight(name, color, position, constant, linear, exponent, renderingEngine, LightComponent::SPOT_LIGHT)
{
	setDirection(direction);
	setAngle(angle);
	calculateDepthMVP();
}

SpotLight::~SpotLight()
{
	disableShadowCasting();
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

void SpotLight::enableShadowCasting(bool useSoftShadows/* = false*/)
{
	m_castsShadows = true;
	m_softShadows = useSoftShadows;
	m_cullingObject = new FrustumComponent((1.0f - m_angle) * 180.0f, 0.01f, getRange(0.01f));
}

void SpotLight::disableShadowCasting()
{
	m_castsShadows = false;

	if (m_cullingObject != nullptr)
		delete m_cullingObject;

	m_cullingObject = nullptr;
}

void SpotLight::updateUniforms(Shader* shader)
{

	shader->setUniform("lightColor", getLight());
	shader->setUniform("lightPosition", getPosition());
	shader->setUniform("lightConstant", m_constant);
	shader->setUniform("lightLinear", m_linear);
	shader->setUniform("lightExponent", m_exponent);
	shader->setUniform("cutOff", m_angle);
	shader->setUniform("lightDir", m_direction);
}

void SpotLight::calculateDepthMVP()
{
	std::cout << getRange(m_renderingEngine->getAmbientIntensity()) << std::endl;
	glm::mat4 depthProjectionMatrix = glm::perspective(180.0f * (1.0f - m_angle), 1.0f, 0.01f, getRange(m_renderingEngine->getAmbientIntensity()));
	glm::mat4 depthViewMatrix = glm::lookAt(m_position, m_position + m_direction, glm::vec3(0.0f, 1.0f, 0.0f));
	m_depthMVP = depthProjectionMatrix * depthViewMatrix;
}

FrustumComponent* SpotLight::getCullingObject()
{
	return m_cullingObject;
}
