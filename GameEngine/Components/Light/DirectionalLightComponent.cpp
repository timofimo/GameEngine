#include "DirectionalLightComponent.h"

/*local includes*/
#include "../Engine.h"
#include "../GameObjects/GameObject.h"

DirectionalLightComponent::DirectionalLightComponent() : LightComponent("DirectionalLightComponent", DIRECTIONAL_LIGHT)
{
}

DirectionalLightComponent::~DirectionalLightComponent()
{
	if (m_cullingObject != nullptr) delete m_cullingObject;
}

void DirectionalLightComponent::enableShadowCasting(bool softShadows /*= false*/, float shadowSize /*= 20.0f*/, float shadowZNear /*= -20.0f*/, float shadowZFar /*= 20.0f*/)
{
	m_castsShadows = true;
	m_castsSoftShadows = softShadows;
	m_shadowSize = shadowSize;
	m_shadowZNear = shadowZNear;
	m_shadowZFar = shadowZFar;
}

void DirectionalLightComponent::disableShadowCasting()
{
	m_castsShadows = false;
}

glm::mat4 DirectionalLightComponent::getShadowMVP()
{
	glm::vec3 position = RenderingEngine::get().getCamera()->getLocalTransform().position();
	glm::vec3 direction = m_parent->getWorldTransform(false).forward();

	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-m_shadowSize, m_shadowSize, -m_shadowSize, m_shadowSize, m_shadowZNear, m_shadowZFar);
	glm::mat4 depthViewMatrix = glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
	return depthProjectionMatrix * depthViewMatrix;
}

FrustumComponent* DirectionalLightComponent::getCullingObject()
{
	if (m_cullingObject == nullptr)
	{
		m_cullingObject = new FrustumComponent(-m_shadowSize, m_shadowSize, -m_shadowSize, m_shadowSize, m_shadowZNear, m_shadowZFar);
		m_cullingObject->setParent(m_parent);
	}

	Transform t = RenderingEngine::get().getCamera()->getLocalTransform();
	if (t.hasChanged(false))
	{
		t.setRotation(m_parent->getWorldTransform(false).rotation());
		m_cullingObject->updatePlanes(&t);
	}
	return m_cullingObject;
}

void DirectionalLightComponent::updateUniforms(Shader* shader, bool updateShadowUniforms)
{
	shader->setUniform("lightColor", getLight());
	shader->setUniform("lightDir", getDirection());
	if (updateShadowUniforms)
	{
		shader->setUniform("depthBiasMVP", m_shadowBiasMatrix * getShadowMVP());
		shader->setSamplerID("shadowMap", 1);
		shader->setUniform("softShadow", m_castsSoftShadows);
	}
}
