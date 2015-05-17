#include "MeshRenderer.h"

std::map<std::string, MeshRenderer*> MeshRenderer::m_meshRenderers;

#include "../Shader.h"
#include "../GameObject.h"
#include "../RenderingEngine.h"

MeshRenderer::MeshRenderer(std::string name, std::string meshFile, std::string textureFile, RenderingEngine* renderingEngine) : RenderComponent(name)
{
	m_meshFile = meshFile;
	m_textureFile = textureFile;
	m_mesh = Mesh::getMesh(m_meshFile);
	m_texture = Texture::getTexture(m_textureFile);

	renderingEngine->addMeshRenderer(this);
}

MeshRenderer::~MeshRenderer()
{
	
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

void MeshRenderer::render(Shader* shader)
{
	/*shader->updateModelMatrix(m_parents[0]->getWorldTransform().modelMatrix());
	if (m_texture)
		m_texture->bind(0);
	m_mesh->bind();
	m_mesh->draw();
	m_mesh->unbind();*/
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
