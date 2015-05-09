#pragma once

/*local includes*/
#include "glm/glm.hpp"

/*utility includes*/
#include <iostream>

class Vertex
{
public:
	bool operator== (Vertex& other)
	{
		return position == other.position &&
			uv == other.uv &&
			normal == other.normal;
	}

	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;

	void print()
	{
		std::cout << "Position(" << position.x << ", " << position.y << ", " << position.z << ") " <<
			"UV(" << uv.x << ", " << uv.y << ") " <<
			"Normal(" << normal.x << ", " << normal.y << ", " << normal.z << ")" << std::endl;
	}
};