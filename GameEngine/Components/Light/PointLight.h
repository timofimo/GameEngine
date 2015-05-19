#pragma once

/*local includes*/
#include "LightComponent.h"

class PointLight : public LightComponent
{
protected:
	float m_constant, m_linear, m_exponent;
public:
	PointLight(std::string name, glm::vec3 color, float constant, float linear, float exponent, RenderingEngine* renderingEngine);
	~PointLight();

	float getConstant();
	void setConstant(float constant);
	float getLinear();
	void setLinear(float linear);
	float getExponent();
	void setExponent(float exponent);
	glm::vec3 getAttenuation();
	void setAttenuation(glm::vec3 attenuation);
	float getRange();
};

