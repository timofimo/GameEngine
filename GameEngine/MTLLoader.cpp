#include "MTLLoader.h"

/*utility includes*/
#include <fstream>
#include <string>
#include <vector>

MTLLoader::MTLLoader()
{
}


MTLLoader::~MTLLoader()
{
}

void MTLLoader::loadMTL(std::string filepath)
{
	std::ifstream file(filepath);
	if (file.is_open())
	{
		std::string line = "";
		while (std::getline(file, line))
		{
			if (line[0] == '#')
				continue;

			std::vector<std::string> substrings;
			size_t pos = 0;
			while (pos != UINT_MAX)
			{
				pos = line.find(" ");
				substrings.push_back(line.substr(0, pos));
				line.erase(0, pos + 1);
			}

			if (substrings[0] == "newmtl")
			{
				//push current material
				//create new material by name substrings[1]
				continue;
			}

			if (substrings[0] == "map_Bump")
			{
				//normal map
				continue;
			}

			if (substrings[0] == "map_Kd")
			{
				//diffuse map
				continue;
			}

			if (substrings[0] == "map_Ks")
			{
				//specular map
				continue;
			}
		}
		file.close();
	}
	else
	{
		std::cout << "Failed to open MTL file: " << filepath.c_str() << std::endl;
	}
}
