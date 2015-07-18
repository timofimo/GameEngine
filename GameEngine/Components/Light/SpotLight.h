#pragma once

/*local includes*/
#include "PointLight.h"
#include "../Components/Physics/FrustumComponent.h"

class SpotLight : public PointLight
{
protected:
	glm::vec3 m_direction;
	float m_angle;
	FrustumComponent* m_cullingObject;

	void calculateDepthMVP();
public:
	SpotLight(std::string name, glm::vec3 color, glm::vec3 position, glm::vec3 direction, float angle, float constant, float linear, float exponent, RenderingEngine* renderingEngine);
	~SpotLight();

	glm::vec3 getDirection();
	void setDirection(glm::vec3 direction);
	float getAngle();
	void setAngle(float angle);

	void enableShadowCasting(bool useSoftShadows = false);
	void disableShadowCasting();

	void updateUniforms(Shader* shader);

	FrustumComponent* getCullingObject();
};

