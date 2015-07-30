#include "MeshRenderer.h"

std::map<std::string, MeshRenderer*> MeshRenderer::m_meshRenderers;

#include "../Shaders/Shader.h"
#include "../GameObjects/GameObject.h"
#include "../RenderingEngine.h"

MeshRenderer::MeshRenderer(std::string name, std::string meshFile, std::string textureFile, RenderingEngine* renderingEngine) : RenderComponent(name, renderingEngine)
{
	m_meshFile = meshFile;
	m_textureFile = textureFile;
	m_mesh = Mesh::getMesh(m_meshFile);
	m_texture = Texture::getTexture(m_textureFile);

	renderingEngine->addMeshRenderer(this);
}

MeshRenderer::~MeshRenderer()
{
	m_mesh->release();
	m_texture->release();
}

MeshRenderer* MeshRenderer::getMeshRenderer(std::string meshFile, std::string textureFile, RenderingEngine* renderingEngine, GameObject* parent)
{
	std::map<std::string, MeshRenderer*>::iterator it = m_meshRenderers.find(meshFile + textureFile);
	MeshRenderer* result = nullptr;
	if (it == m_meshRenderers.end())
	{
		m_meshRenderers[meshFile + textureFile] = result = new MeshRenderer(meshFile + textureFile, meshFile, textureFile, renderingEngine);
	}
	else
		result = it->second;

	return result;
}

Mesh* MeshRenderer::getMesh()
{
	return m_mesh;
}

Texture* MeshRenderer::getTexture()
{
	return m_texture;
}

std::vector<GameObject*> MeshRenderer::getParents()
{
	return m_parents;
}

bool MeshRenderer::parentsTransformChanged()
{
	bool result = false;
	for each (GameObject* parent in m_parents)
	{
		result |= parent->transformChanged(false);
	}
	return result;
}

void MeshRenderer::removeParent(GameObject* parent)
{
	std::vector<GameObject*>::iterator it = std::find(m_parents.begin(), m_parents.end(), parent);
	if (it == m_parents.end())
	{
		std::cout << "ERROR MESHRENDERER: " << parent->getName().c_str() << " tried to delete a component that doesn't exist" << std::endl;
	}
	else
	{
		m_parents.erase(it);
		if (m_parents.size() <= 0)
		{
			m_renderingEngine->removeMeshRenderer(this);
			delete this;
		}
	}
}
