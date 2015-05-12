#include "MeshRenderer.h"

#include "../Shader.h"
#include "../GameObject.h"

MeshRenderer::MeshRenderer(std::string name, std::string meshFile, std::string textureFile) : RenderComponent(name)
{
	m_meshFile = meshFile;
	m_textureFile = textureFile;
	m_mesh = ResourceManager::getMesh(m_meshFile, this);
	m_texture = ResourceManager::getTexture(m_textureFile);
}

MeshRenderer::~MeshRenderer()
{
	ResourceManager::releaseMesh(m_meshFile, this);
}

void MeshRenderer::render(Shader* shader)
{
	shader->updateModelMatrix(m_parent->getWorldTransform().modelMatrix());
	if (m_texture)
		m_texture->bind(0);
	m_mesh->draw();
}