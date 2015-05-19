#pragma once

/*local includes*/
#include "PointLight.h"

class SpotLight : public PointLight
{
protected:
	glm::vec3 m_direction;
	float m_angle;
public:
	SpotLight(std::string name, glm::vec3 color, glm::vec3 direction, float angle, float constant, float linear, float exponent, RenderingEngine* renderingEngine);
	~SpotLight();

	glm::vec3 getDirection();
	void setDirection(glm::vec3 direction);
	float getAngle();
	void setAngle(float angle);
};

