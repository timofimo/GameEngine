#pragma once

class RenderingEngine;

/*local includes*/
#include "../GameComponent.h"


class LightComponent : public GameComponent
{
protected:
	RenderingEngine* m_renderingEngine;
	glm::vec3 m_color;
	float m_intensity;

public:
	LightComponent(std::string name, glm::vec3 color, float intensity, RenderingEngine* renderingEngine);
	~LightComponent();

	glm::vec3 getColor();
	void setColor(glm::vec3 color);
	float getIntensity();
	void setIntensity(float intensity);
	glm::vec3 getLight();

private:
	void start(){}
	void update(float deltaTime){}
};

