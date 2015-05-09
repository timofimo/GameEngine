#pragma once

/*local includes*/
#include "Vertex.h"

/*utility includes*/
#include <vector>

class OBJLoader
{
private:
	std::vector<glm::vec3> m_vertexPositions;
	std::vector<glm::vec2> m_vertexTexCoords;
	std::vector<glm::vec3> m_vertexNormals;
	std::vector<unsigned int> m_indices;

public:
	OBJLoader();
	~OBJLoader();

	void loadOBJ(const std::string obj, std::vector<Vertex>& returnVertices, std::vector<unsigned int>& returnIndices);

private:
	glm::vec3 loadVec3(std::string line);
	glm::vec2 loadVec2(std::string line);
	std::vector<unsigned int> loadIndex(std::string line);
};

