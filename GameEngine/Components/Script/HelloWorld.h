#pragma once

/*local includes*/
#include "ScriptComponent.h"

class HelloWorld : public ScriptComponent
{
private:
	std::string printString;
	float passedTime;
public:
	HelloWorld() : ScriptComponent("Hello World")
	{}
	~HelloWorld(){}

	void start()
	{
		passedTime = 0.0f;
		printString = "Hello World!!!";
	}

	void update(float deltaTime)
	{
		passedTime += deltaTime;
		if (passedTime > 3.0f)
		{
			std::cout << printString.c_str() << std::endl;
			passedTime = 0.0f;
		}
	}
};