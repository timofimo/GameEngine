#pragma once

/*local includes*/
#include "LightComponent.h"
#include "../Components/Physics/SphereComponent.h"

class PointLightComponent : public LightComponent
{
private:
	float m_exponent;
	float m_linear;
	float m_constant;
	SphereComponent* m_cullingObject;
public:
	PointLightComponent(float exponent = 1.0f, float linear = 0.0f, float constant = 0.0f);
	~PointLightComponent();

	float getRange(float lightAmount);

	glm::mat4 getShadowMVP();

	SphereComponent* getCullingObject();

	void updateUniforms(Shader* shader, bool updateShadowUniforms);

private:
	void update(float deltaTime){};
};

