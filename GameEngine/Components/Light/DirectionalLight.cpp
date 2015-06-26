#include "DirectionalLight.h"

/*local includes*/
#include "../GameObject.h"
#include "../Shaders/InstanceDirectionalShader.h"
#include "../Shaders/InstanceDirectionalShadowShader.h"

DirectionalLight::DirectionalLight(std::string name, glm::vec3 color, float intensity, glm::vec3 direction, RenderingEngine* renderingEngine) : LightComponent(name, color, intensity, renderingEngine, LightComponent::DIRECTIONAL_LIGHT)
{
	m_direction = direction;
}


DirectionalLight::~DirectionalLight()
{
}

glm::vec3 DirectionalLight::getDirection()
{
	return m_direction;
}

void DirectionalLight::setDirection(glm::vec3 direction)
{
	m_direction = glm::normalize(direction);
}

void DirectionalLight::setShadowMap(GLuint map)
{
	this->m_shadowMap = map;
}

void DirectionalLight::updateUniforms(Shader* shader)
{
	InstanceDirectionalShader* s = (InstanceDirectionalShader*)shader;
	s->setLightColor(getLight());
	s->setLightDirection(m_direction);
}

void DirectionalLight::updateShadowUniforms(Shader* shader, glm::vec3 camPosition)
{
	if (m_shadowMap == NULL)
	{
		updateUniforms(shader);
		return;
	}

	// Compute the MVP matrix from the light's point of view
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::vec3 position = camPosition + (-m_direction * 10.0f);
	glm::vec3 direction = position + m_direction;
	glm::mat4 depthViewMatrix = glm::lookAt(position, direction, glm::vec3(0, 1, 0));
	glm::mat4 depthModelMatrix = glm::mat4(1.0);
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	InstanceDirectionalShadowShader* s = (InstanceDirectionalShadowShader*)shader;
	s->setDepthVP(depthMVP);
	//glUniformMatrix4fv(s, 1, GL_FALSE, &depthMVP[0][0])
}
