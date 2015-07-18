#include "DirectionalLight.h"

/*local includes*/
#include "../GameObject.h"
#include "../RenderingEngine.h"

DirectionalLight::DirectionalLight(std::string name, glm::vec3 color, float intensity, glm::vec3 direction, RenderingEngine* renderingEngine) : LightComponent(name, color, intensity, renderingEngine, LightComponent::DIRECTIONAL_LIGHT)
{
	m_direction = direction;
	calculateDepthMVP();
}


DirectionalLight::~DirectionalLight()
{
	disableShadowCasting();
}

glm::vec3 DirectionalLight::getDirection()
{
	return m_direction;
}

void DirectionalLight::setDirection(glm::vec3 direction)
{
	m_direction = glm::normalize(direction);
}

void DirectionalLight::enableShadowCasting(bool useSoftShadows/* = false*/, float shadowSize /*= 20.0f*/, float zNear /*= -20.0f*/, float zFar /*= 20.0f*/)
{
	m_castsShadows = true;
	m_softShadows = useSoftShadows;
	m_shadowSize = shadowSize;
	m_shadowZNear = zNear;
	m_shadowZFar = zFar;	

	m_cullingObject = new FrustumComponent(-shadowSize, shadowSize, -shadowSize, shadowSize, zNear, zFar);
	m_cullingObject->setParent(m_parent);
}

void DirectionalLight::disableShadowCasting()
{
	m_castsShadows = false;

	if (m_cullingObject != nullptr)
		delete m_cullingObject;

	m_cullingObject = nullptr;
}

void DirectionalLight::updateUniforms(Shader* shader)
{
	shader->setUniform("lightColor", getLight());
	shader->setUniform("lightDir", m_direction);
}

void DirectionalLight::calculateDepthMVP()
{
	glm::vec3 position = m_renderingEngine->getCamera()->getLocalTransform().position();
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-m_shadowSize, m_shadowSize, -m_shadowSize, m_shadowSize, m_shadowZNear, m_shadowZFar);
	glm::mat4 depthViewMatrix = glm::lookAt(position, position + m_direction, glm::vec3(0.0f, 1.0f, 0.0f));
	m_depthMVP = depthProjectionMatrix * depthViewMatrix;
}

FrustumComponent* DirectionalLight::getCullingObject()
{
	return m_cullingObject;
}
