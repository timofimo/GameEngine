#pragma once

/*local includes*/
#include "LightComponent.h"

class PointLight : public LightComponent
{
protected:
	glm::vec3 m_position;
	float m_constant, m_linear, m_exponent;

	virtual void calculateDepthMVP(){}
public:
	PointLight(std::string name, glm::vec3 color, glm::vec3 position, float constant, float linear, float exponent, RenderingEngine* renderingEngine, LightComponent::LightTypes type = LightComponent::POINT_LIGHT);
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
	float getRange(float lightAmount);
	glm::vec3 getPosition();

	virtual void updateUniforms(Shader* shader);
};

