#pragma once

/*local includes*/
#include "RenderComponent.h"
#include "../Mesh.h"
#include "../Texture.h"


class MeshRenderer : public RenderComponent
{
private:
	static std::map<std::string, MeshRenderer*> m_meshRenderers;
	std::string m_meshFile, m_textureFile;
	Mesh* m_mesh;
	Texture* m_texture;
public:
	MeshRenderer(std::string name, std::string meshFile, std::string textureFile, RenderingEngine* renderingEngine);
	~MeshRenderer();

	static MeshRenderer* getMeshRenderer(std::string meshFile, std::string textureFile, RenderingEngine* renderingEngine, GameObject* parent);

	void render(Shader* shader);

	Mesh* getMesh();
	Texture* getTexture();

	std::vector<GameObject*> getParents();
	bool parentsTransformChanged();

	void removeParent(GameObject* parent);
};