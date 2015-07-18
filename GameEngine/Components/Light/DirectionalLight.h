#pragma once

/*local includes*/
#include "LightComponent.h"
#include "../Components/Physics/FrustumComponent.h"

class DirectionalLight : public LightComponent
{
protected:
	glm::vec3 m_direction;
	FrustumComponent* m_cullingObject;
	float m_shadowSize, m_shadowZNear, m_shadowZFar;
public:
	DirectionalLight(std::string name, glm::vec3 color, float intensity, glm::vec3 direction, RenderingEngine* renderingEngine);
	~DirectionalLight();

	glm::vec3 getDirection();
	void setDirection(glm::vec3 direction);

	void enableShadowCasting(bool useSoftShadows = false, float shadowSize = 20.0f, float zNear = -20.0f, float zFar = 20.0f);
	void disableShadowCasting();

	void updateUniforms(Shader* shader);
	void calculateDepthMVP();

	FrustumComponent* getCullingObject();
};

