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
	static OBJLoader m_objLoader;
	static std::map<std::string, Mesh*> m_meshes;
	static std::map<std::string, Texture*> m_textures;

public:
	ResourceManager();
	~ResourceManager();

	static Mesh* getMesh(const std::string file, RenderComponent* component);
	static Texture* getTexture(const std::string file);

	static void releaseMesh(const std::string file, RenderComponent* component);
};

