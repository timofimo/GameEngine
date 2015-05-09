#pragma once

/*local includes*/
#include "ScriptComponent.h"

class ParentScript : public ScriptComponent
{
public:
	ParentScript();
	~ParentScript();

	void update(float deltaTime);
};

