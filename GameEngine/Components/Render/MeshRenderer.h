#pragma once

/*local includes*/
#include "RenderComponent.h"
#include "../ResourceManager.h"

class MeshRenderer : public RenderComponent
{
private:
	std::string m_meshFile, m_textureFile;
	Mesh* m_mesh;
	Texture* m_texture;
public:
	MeshRenderer(std::string name, std::string meshFile, std::string textureFile);
	~MeshRenderer();

	void render(Shader* shader);
};