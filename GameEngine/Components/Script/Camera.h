#pragma once

/*local includes*/
#include "ScriptComponent.h"

class Camera : public ScriptComponent
{
private:
	int counter;
public:
	Camera();
	~Camera();

	void update(float deltaTime);

	void setActive();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getVPMatrix();
};

