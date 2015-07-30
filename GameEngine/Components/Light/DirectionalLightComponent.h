#pragma once

/*local includes*/
#include "LightComponent.h"
#include "../Components/Physics/FrustumComponent.h"

class DirectionalLightComponent : public LightComponent
{
private:
	float m_shadowSize;
	float m_shadowZNear;
	float m_shadowZFar;
	FrustumComponent* m_cullingObject;
public:
	DirectionalLightComponent();
	~DirectionalLightComponent();

	void enableShadowCasting(bool softShadows = false, float shadowSize = 20.0f, float shadowZNear = -20.0f, float shadowZFar = 20.0f);
	void disableShadowCasting();

	glm::mat4 getShadowMVP();

	FrustumComponent* getCullingObject();

	void updateUniforms(Shader* shader, bool updateShadowUniforms);
};

