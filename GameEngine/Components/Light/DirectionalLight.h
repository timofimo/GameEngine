#pragma once

/*local includes*/
#include "LightComponent.h"

class DirectionalLight : public LightComponent
{
protected:
	glm::vec3 m_direction;
	GLuint m_shadowMap = NULL;
public:
	DirectionalLight(std::string name, glm::vec3 color, float intensity, glm::vec3 direction, RenderingEngine* renderingEngine);
	~DirectionalLight();

	glm::vec3 getDirection();
	void setDirection(glm::vec3 direction);
	void setShadowMap(GLuint map);

	void updateUniforms(Shader* shader);
	void updateShadowUniforms(Shader* shader, glm::vec3 camPosition);
};

