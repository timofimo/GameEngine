#pragma once

/*local includes*/
#include "GameObject.h"
#include "../Components/Light/DirectionalLightComponent.h"

class DirectionalLight : public GameObject
{
private:
	DirectionalLightComponent* m_directionalLight;
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 color, float intensity);
	DirectionalLight(glm::vec3 color, float intensity, bool castsSoftShadows, float shadowSize, float shadowZNear, float shadowZFar);
	~DirectionalLight();

	void setLightParameters(glm::vec3 color, float intensity);
	void setShadowParameters(bool castsShadows, bool castsSoftShadows, float shadowSize, float shadowZNear, float shadowZFar);
};

