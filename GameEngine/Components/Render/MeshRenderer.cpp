#include "MeshRenderer.h"

#include "../Shader.h"
#include "../GameObject.h"

MeshRenderer::MeshRenderer(std::string name, std::string meshFile, std::string textureFile) : RenderComponent(name)
{
	m_mesh = Mesh::loadMesh(meshFile);
	m_texture = new Texture(textureFile);
}

MeshRenderer::~MeshRenderer()
{
	m_mesh->release();
}

void MeshRenderer::render(Shader* shader)
{
	shader->updateModelMatrix(m_parent->getWorldTransform().modelMatrix());
	if (m_texture)
		m_texture->bind(0);
	m_mesh->draw();
}