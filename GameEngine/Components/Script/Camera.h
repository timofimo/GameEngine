#pragma once

/*local includes*/
#include "ScriptComponent.h"

class Camera : public ScriptComponent
{
public:
	Camera();
	~Camera();

	void update(float deltaTime);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getVPMatrix();
	glm::vec3 getPosition();
};

