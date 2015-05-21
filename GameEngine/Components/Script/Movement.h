#pragma once

/*local includes*/
#include "ScriptComponent.h"

class Movement : public ScriptComponent
{
private:
	glm::vec3 m_startPosition;
	float m_passedTime;
public:
	Movement();
	~Movement();

	void start();
	void update(float deltaTime);
};

