#pragma once

/*local includes*/
#include "LightComponent.h"

class DirectionalLight : public LightComponent
{
protected:
	glm::vec3 m_direction;
public:
	DirectionalLight(std::string name, glm::vec3 color, float intensity, glm::vec3 direction, RenderingEngine* renderingEngine);
	~DirectionalLight();

	glm::vec3 getDirection();
	void setDirection(glm::vec3 direction);

	void updateUniforms(Shader* shader);
};

