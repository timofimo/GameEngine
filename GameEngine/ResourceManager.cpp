#include "ResourceManager.h"

std::map<std::string, Texture*> ResourceManager::m_textures;


ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}

Texture* ResourceManager::getTexture(const std::string file)
{
	if (m_textures.find(file) == m_textures.end())
	{
		m_textures[file] = new Texture(file);
	}

	return m_textures[file];
}
