#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(std::string name, glm::vec3 color, float intensity, glm::vec3 direction, RenderingEngine* renderingEngine) : LightComponent(name, color, intensity, renderingEngine)
{
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
