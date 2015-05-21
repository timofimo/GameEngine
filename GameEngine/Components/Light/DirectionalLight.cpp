#include "DirectionalLight.h"

/*local includes*/
#include "../GameObject.h"
#include "../Shaders/InstanceDirectionalShader.h"

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

void DirectionalLight::updateUniforms(Shader* shader)
{
	InstanceDirectionalShader* s = (InstanceDirectionalShader*)shader;
	s->setLightColor(getLight());
	s->setLightDirection(m_direction);
}
