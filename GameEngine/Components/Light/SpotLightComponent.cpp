#include "SpotLightComponent.h"

/*local includes*/
#include "../GameObjects/GameObject.h"

SpotLightComponent::SpotLightComponent(float cutoff/* = 0.7f*/, float exponent/* = 1.0f*/, float linear/* = 0.0f*/, float constant/* = 0.0f*/) : LightComponent("SpotLightComponent", SPOT_LIGHT)
{
	m_cutoff = cutoff;
	m_exponent = exponent;
	m_linear = linear;
	m_constant = constant;
}


SpotLightComponent::~SpotLightComponent()
{
	if (m_cullingObject != nullptr) delete m_cullingObject;
}

void SpotLightComponent::setExponent(float exponent)
{
	m_exponent = exponent;
}

void SpotLightComponent::setCutoff(float cutoff)
{
	m_cutoff = cutoff;
}

float SpotLightComponent::getRange(float lightAmount)
{
	if (m_exponent == 0)
		return 0.0f;

	float a = m_exponent;
	float b = m_linear;
	float c = 0.0f;

	if (lightAmount <= 0.0f || lightAmount > 1.0f)
	{
		float colorMax = getColor().r > getColor().g ? (getColor().r > getColor().b ? getColor().r : getColor().b) : (getColor().g > getColor().b ? getColor().g : getColor().b);// highest of the three components		
		c = m_constant - 256.0f * m_intensity * colorMax;
	}
	else
	{
		float m = 1 / lightAmount;
		c = m_constant - m;
	}

	float d = b * b - 4 * a * c;
	if (d < 0.0f)
		return 0.0f;
	return ((-b + glm::sqrt(d)) / (2 * a));
}

glm::mat4 SpotLightComponent::getShadowMVP()
{
	Transform t = m_parent->getWorldTransform(false);
	std::cout << getRange(0.1f) << std::endl;
	glm::mat4 depthProjectionMatrix = glm::perspective(180.0f * (1.0f - m_cutoff), 1.0f, 0.01f, getRange(0.1f));
	glm::mat4 depthViewMatrix = glm::lookAt(t.position(), t.position() + t.forward(), glm::vec3(0.0f, 1.0f, 0.0f));
	return depthProjectionMatrix * depthViewMatrix;
}

FrustumComponent* SpotLightComponent::getCullingObject()
{
	Transform t = m_parent->getWorldTransform(false);
	if (t.hasChanged(false) || m_cullingObject == nullptr)
	{
		if (m_cullingObject == nullptr)
			m_cullingObject = new FrustumComponent(180.0f * (1.0f - m_cutoff), 0.01f, getRange(0.01f));
		else
		{
			delete m_cullingObject;
			m_cullingObject = new FrustumComponent(180.0f * (1.0f - m_cutoff), 0.01f, getRange(0.01f));
		}
		m_cullingObject->setParent(m_parent);
	}
	return m_cullingObject;
}

void SpotLightComponent::updateUniforms(Shader* shader, bool updateShadowUniforms)
{
	shader->setUniform("lightColor", getLight());
	shader->setUniform("lightPosition", getPosition());
	shader->setUniform("lightConstant", m_constant);
	shader->setUniform("lightLinear", m_linear);
	shader->setUniform("lightExponent", m_exponent);
	shader->setUniform("cutOff", m_cutoff);
	shader->setUniform("lightDir", getDirection());
	if (updateShadowUniforms)
	{
		shader->setUniform("depthBiasMVP", m_shadowBiasMatrix * getShadowMVP());
		shader->setSamplerID("shadowMap", 1);
		shader->setUniform("softShadow", m_castsSoftShadows);
	}
}