#include "ResourceManager.h"


OBJLoader ResourceManager::m_objLoader;
std::map<std::string, Mesh*> ResourceManager::m_meshes;
std::map<std::string, Texture*> ResourceManager::m_textures;


ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}

Mesh* ResourceManager::getMesh(const std::string file, RenderComponent* component)
{
	if (m_meshes.find(file) == m_meshes.end())
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		m_objLoader.loadOBJ(file, vertices, indices);
		m_meshes[file] = new Mesh(file, vertices, indices);
	}
	m_meshes[file]->addComponent(component);

	return m_meshes[file];
}

Texture* ResourceManager::getTexture(const std::string file)
{
	if (m_textures.find(file) == m_textures.end())
	{
		m_textures[file] = new Texture(file);
	}

	return m_textures[file];
}

void ResourceManager::releaseMesh(const std::string file, RenderComponent* component)
{
	unsigned int result = m_meshes.find(file)->second->releaseComponent(component);
	if (result == 0)
	{
		delete m_meshes.find(file)->second;
		m_meshes.erase(file);
	}
}
