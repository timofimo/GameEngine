#pragma once

/*local includes*/
#include "LightComponent.h"
#include "../Components/Physics/FrustumComponent.h"

class SpotLightComponent : public LightComponent
{
private:
	float m_exponent;
	float m_linear;
	float m_constant;
	float m_cutoff;
	FrustumComponent* m_cullingObject;
public:
	SpotLightComponent(float cutoff = 0.7f, float exponent = 1.0f, float linear = 0.0f, float constant = 0.0f);
	~SpotLightComponent();

	void setExponent(float exponent);
	void setCutoff(float cutoff);

	float getRange(float lightAmount);

	glm::mat4 getShadowMVP();

	FrustumComponent* getCullingObject();

	void updateUniforms(Shader* shader, bool updateShadowUniforms);

private:
	void update(float deltaTime){};
};

