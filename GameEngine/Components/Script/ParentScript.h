#pragma once

/*local includes*/
#include "ScriptComponent.h"

class ParentScript : public ScriptComponent
{
private:
	float m_time;
public:
	ParentScript();
	~ParentScript();

	void update(float deltaTime);
};

