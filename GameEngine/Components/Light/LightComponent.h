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

public:
	LightComponent(std::string name, glm::vec3 color, float intensity, RenderingEngine* renderingEngine, LightTypes type);
	~LightComponent();

	glm::vec3 getColor();
	void setColor(glm::vec3 color);
	float getIntensity();
	void setIntensity(float intensity);
	glm::vec3 getLight();
	LightTypes getType();

	virtual void updateUniforms(Shader* shader) = 0;

private:
	void start(){}
	void update(float deltaTime){}
};

