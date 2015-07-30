#include "PointLightComponent.h"

/*local includes*/
#include "../GameObjects/GameObject.h"

PointLightComponent::PointLightComponent(float exponent/* = 1.0f*/, float linear/* = 0.0f*/, float constant/* = 0.0f*/) : LightComponent("SpotLightComponent", POINT_LIGHT)
{
}


PointLightComponent::~PointLightComponent()
{
	if (m_cullingObject != nullptr) delete m_cullingObject;
}

float PointLightComponent::getRange(float lightAmount)
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

glm::mat4 PointLightComponent::getShadowMVP()
{
	return glm::mat4();
}

SphereComponent* PointLightComponent::getCullingObject()
{
	Transform t = m_parent->getWorldTransform(false);
	if (t.hasChanged(false))
	{
		if (m_cullingObject == nullptr)
			m_cullingObject = new SphereComponent(t.position(), getRange(0.01f));
		else
			m_cullingObject = &SphereComponent(t.position(), getRange(0.01f));
	}
	return m_cullingObject;
}

void PointLightComponent::updateUniforms(Shader* shader, bool updateShadowUniforms)
{
	shader->setUniform("lightColor", getLight());
	shader->setUniform("lightPosition", getPosition());
	shader->setUniform("lightConstant", m_constant);
	shader->setUniform("lightLinear", m_linear);
	shader->setUniform("lightExponent", m_exponent);
}
