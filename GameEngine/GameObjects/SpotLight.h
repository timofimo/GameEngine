#pragma once

/*local includes*/
#include "GameObject.h"
#include "../Components/Light/SpotLightComponent.h"

class SpotLight : public GameObject
{
private:
	SpotLightComponent* m_spotLight;
public:
	SpotLight();
	SpotLight(glm::vec3 color, float intensity, float cutoff, float exponent);
	SpotLight(glm::vec3 color, float intensity, float cutoff, float exponent, glm::vec3 position, glm::vec3 rotation);
	SpotLight(glm::vec3 color, float intensity, float cutoff, float exponent, glm::vec3 position, glm::vec3 rotation, bool castsSoftShadows);
	~SpotLight();

	void setLightParameters(glm::vec3 color, float intensity, float cutoff, float exponent);
	void setShadowParameters(bool castsShadows, bool castsSoftShadows);
};

