#pragma once

/*local includes*/
#include "../GameComponent.h"
#include "../Components/Physics/PhysicsComponent.h"

enum LightType
{
	DIRECTIONAL_LIGHT,
	SPOT_LIGHT,
	POINT_LIGHT
};

class LightComponent : public GameComponent
{
protected:
	float m_intensity;
	glm::vec3 m_color;
	LightType m_type;

	bool m_castsShadows;
	bool m_castsSoftShadows;
	int m_shadowMapIndex;

	glm::mat4 m_shadowBiasMatrix = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);
public:
	LightComponent(std::string name, LightType type);
	virtual ~LightComponent();

	void setIntensity(float intensity);
	float getIntensity();

	void setColor(glm::vec3 color);
	glm::vec3 getColor();
	glm::vec3 getLight();
	LightType getType();

	virtual void enableShadowCasting(bool softShadows = false);
	virtual void disableShadowCasting();

	void setShadowMapIndex(int index);
	int getShadowMapIndex();

	bool castsShadows();
	bool castsSoftShadows();

	virtual glm::mat4 getShadowMVP() = 0;

	virtual glm::vec3 getPosition();
	virtual glm::vec3 getDirection();
	virtual float getRange(float lightAmount);

	virtual PhysicsComponent* getCullingObject() = 0;

	virtual void updateUniforms(Shader* shader, bool updateShadowUniforms) = 0;
private:
	void start(){}
	void update(float deltaTime){};
};

