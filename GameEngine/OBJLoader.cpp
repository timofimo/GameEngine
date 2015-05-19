#include "OBJLoader.h"

/*utility includes*/
#include <fstream>
#include <string>
#include <iostream>

OBJLoader::OBJLoader()
{
}


OBJLoader::~OBJLoader()
{
}

void OBJLoader::loadOBJ(const std::string obj, std::vector<Vertex>& returnVertices, std::vector<unsigned int>& returnIndices)
{
	std::ifstream file(obj.c_str());
	if (file.is_open())
	{
		std::string line;
		bool firstObject = true;
		while (std::getline(file, line))
		{
			// step over unused information
			if (line[0] == '#')
				continue;
			if (line[0] == 'm')
			{
				std::cerr << "This OBJ loader can not load mtl files: " << obj << std::endl;
				continue;
			}
			if (line[0] == 'o')
			{
				continue;
			}
			if (line[0] == 's' && line.substr(2, 3) != "off")
			{
				std::cerr << "This OBJ loader can not load smoothing groups: " << obj << std::endl;
				continue;
			}


			// loading the data
			// load vertex position
			if (line[0] == 'v' && line[1] == ' ')
			{
				m_vertexPositions.push_back(loadVec3(line));
				continue;
			}
			// load texCoord
			if (line[0] == 'v' && line[1] == 't')
			{
				m_vertexTexCoords.push_back(loadVec2(line));
				continue;
			}
			// load normal
			if (line[0] == 'v' && line[1] == 'n')
			{
				m_vertexNormals.push_back(loadVec3(line));
				continue;
			}
			//load indices
			if (line[0] == 'f')
			{
				std::vector<unsigned int> indices = loadIndex(line);
				m_indices.insert(m_indices.end(), indices.begin(), indices.end());
			}
		}
		file.close();
	}
	else
	{
		std::cerr << "Could not open OBJ file: " << obj << std::endl;
		return;
	}

	indexModel(returnVertices, returnIndices);
}

glm::vec3 OBJLoader::loadVec3(std::string line)
{
	glm::vec3 result;

	// delete everything up to the first value
	line.erase(0, 2);
	size_t pos = line.find_first_not_of(' ');
	line.erase(0, pos);

	char nData = 1;
	while (pos != UINT_MAX)
	{
		pos = line.find(' ');
		switch (nData)
		{
		case 1:
			result.x = (float)atof(line.substr(0, pos).c_str());
			break;
		case 2:
			result.y = (float)atof(line.substr(0, pos).c_str());
			break;
		case 3:
			result.z = (float)atof(line.substr(0, pos).c_str());
			break;
		case 4:
			std::cerr << "more than 3 components in vec3" << std::endl;
			break;
		default:
			break;
		}
		line.erase(0, pos + 1);
		nData++;
	}

	return result;
}

glm::vec2 OBJLoader::loadVec2(std::string line)
{
	glm::vec2 result;

	// delete everything up to the first value
	line.erase(0, 2);
	size_t pos = line.find_first_not_of(' ');
	line.erase(0, pos);

	char nData = 1;
	while (pos != UINT_MAX)
	{
		pos = line.find(' ');
		switch (nData)
		{
		case 1:
			result.x = (float)atof(line.substr(0, pos).c_str());
			break;
		case 2:
			result.y = -(float)atof(line.substr(0, pos).c_str()); // flips the texture coordinates
			break;
		case 3:
			std::cerr << "more than 2 components in vec2" << std::endl;
			break;
		default:
			break;
		}
		line.erase(0, pos + 1);
		nData++;
	}

	return result;
}

std::vector<unsigned int> OBJLoader::loadIndex(std::string line)
{
	std::vector<unsigned int> indicies;
	std::vector<std::string> substrings;
	size_t pos = 0;
	while (pos != UINT_MAX)
	{
		pos = line.find(' ');
		substrings.push_back(line.substr(0, pos));
		line.erase(0, pos + 1);
	}

	if (substrings.size() != 4)
	{
		std::cerr << "not enough indices" << std::endl;
		return indicies;
	}

	for (int i = 1; i < 4; i++)
	{
		pos = 0;
		while (pos != UINT_MAX)
		{
			pos = substrings[i].find('/');
			indicies.push_back(atoi(substrings[i].substr(0, pos).c_str()) - 1);
			substrings[i].erase(0, pos + 1);
		}
	}

	return indicies;
}

void OBJLoader::indexModel(std::vector<Vertex>& returnVertices, std::vector<unsigned int>& returnIndices)
{
	for (unsigned int i = 0; i < m_indices.size(); i += 3)
	{
		Vertex tempVert;
		tempVert.position = m_vertexPositions[m_indices[i]];
		tempVert.uv = m_vertexTexCoords[m_indices[i + 1]];
		tempVert.normal = m_vertexNormals[m_indices[i + 2]];

		bool vertexExists = false;
		for (unsigned int vertexIndex = 0; vertexIndex < returnVertices.size(); vertexIndex++)
		{
			if (tempVert == returnVertices[vertexIndex])
			{
				vertexExists = true;
				returnIndices.push_back(vertexIndex);
				break;
			}
		}
		if (!vertexExists)
		{
			returnIndices.push_back(returnVertices.size());
			returnVertices.push_back(tempVert);
		}
	}

	returnVertices.shrink_to_fit();
	returnIndices.shrink_to_fit();

	m_vertexPositions.clear();
	m_vertexTexCoords.clear();
	m_vertexNormals.clear();
	m_indices.clear();
}
