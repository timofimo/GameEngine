#pragma once

class OBJLoader;

/*local includes*/
#include "OBJLoader.h"
#include "Mesh.h"
#include "Texture.h"

/*utility includes*/
#include <map>
#include <vector>

class ResourceManager
{
private:
	static std::map<std::string, Texture*> m_textures;

public:
	ResourceManager();
	~ResourceManager();

	static Texture* getTexture(const std::string file);
};

