#pragma once

/*local includes*/
#include "RenderComponent.h"
#include "../Mesh.h"
#include "../Texture.h"

class MeshRenderer : public RenderComponent
{
private:
	Mesh* m_mesh;
	Texture* m_texture;
public:
	MeshRenderer(std::string name, std::string meshFile, std::string textureFile);
	~MeshRenderer();

	void render(Shader* shader);
};