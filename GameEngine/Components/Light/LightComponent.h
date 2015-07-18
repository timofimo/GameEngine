#pragma once

class RenderingEngine;

/*local includes*/
#include "../GameComponent.h"


class LightComponent : public GameComponent
{
public:
	enum LightTypes
	{
		DIRECTIONAL_LIGHT,
		SPOT_LIGHT,
		POINT_LIGHT,
	}; LightTypes m_type;
protected:
	RenderingEngine* m_renderingEngine;
	glm::vec3 m_color;
	float m_intensity;

	glm::mat4 m_depthMVP;
	glm::mat4 biasMatrix = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);
	int m_shadowMapIndex;
	bool m_castsShadows, m_softShadows;
	virtual void calculateDepthMVP() = 0;

public:
	LightComponent(std::string name, glm::vec3 color, float intensity, RenderingEngine* renderingEngine, LightTypes type);
	virtual ~LightComponent();

	glm::vec3 getColor();
	void setColor(glm::vec3 color);
	float getIntensity();
	void setIntensity(float intensity);
	glm::vec3 getLight();
	LightTypes getType();

	virtual void updateUniforms(Shader* shader) = 0;

	glm::mat4 getDepthMVP();
	glm::mat4 getDepthBias();
	void setShadowMapIndex(int map);
	int getShadowMapIndex();

	bool isShadowCaster();
	bool castsSoftShadows();

private:
	void start(){}
	void update(float deltaTime){}
};

