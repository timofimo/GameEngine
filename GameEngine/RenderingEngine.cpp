#include "RenderingEngine.h"

/*local includes*/
#include "SimpleShader.h"
#include "InstanceShader.h"

RenderingEngine::RenderingEngine() : m_display(640, 360, "OpenGL Window", false, false)
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	// initialize all shaders here
	m_shaders[SIMPLE_SHADER] = new SimpleShader();
	m_shaders[INSTANCE_SHADER] = new InstanceShader();

	m_activeShader = m_shaders[SIMPLE_SHADER];
}


RenderingEngine::~RenderingEngine()
{
}

void RenderingEngine::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!m_activeShader || !m_activeCamera)
	{
		std::cout << "There is no active shader or camera" << std::endl;
		return;
	}

	for each (Shader* s in m_shaders)
	{
		s->bind();
		s->updateVPMatrix(m_activeCamera->getVPMatrix());
	}

	for each (MeshRenderer* mr in m_meshRenderers)
	{
		if (mr->getParents().size() > 1)
			renderInstanced(mr);
		else
			render(mr);
	}
	// for each mesh
		// for each texture
			// update model matrix
	// draw

	glfwSwapBuffers(m_display.getWindow());
}

void RenderingEngine::setCamera(Camera* cam)
{
	this->m_activeCamera = cam;
}

void RenderingEngine::setShader(Shader* shader)
{
	this->m_activeShader = shader;
}

bool RenderingEngine::windowShouldClose()
{
	return m_display.shouldClose();
}

void RenderingEngine::addMeshRenderer(MeshRenderer* meshRenderer)
{
	m_meshRenderers.push_back(meshRenderer);
}

void RenderingEngine::render(MeshRenderer* meshRenderer)
{
	SimpleShader* shader = (SimpleShader*)m_shaders[SIMPLE_SHADER];
	shader->bind();
	shader->updateModelMatrix(meshRenderer->getParents()[0]->getWorldTransform().modelMatrix());
	meshRenderer->getMesh()->bind();
	meshRenderer->getTexture()->bind(0);
	meshRenderer->getMesh()->draw();
	meshRenderer->getMesh()->unbind();
	meshRenderer->getTexture()->unbind(0);
}

void RenderingEngine::renderInstanced(MeshRenderer* meshRenderer)
{
	InstanceShader* shader = (InstanceShader*)m_shaders[INSTANCE_SHADER];
	shader->bind();

	float startTime = glfwGetTime();
	std::vector<glm::mat4> instanceMatrices;
	for each (GameObject* parent in meshRenderer->getParents())
	{
		instanceMatrices.push_back(parent->getWorldTransform().modelMatrix());
	}
	//std::cout << "instanceMatrices: " << glfwGetTime() - startTime << std::endl;

	meshRenderer->getMesh()->bind();
	meshRenderer->getTexture()->bind(0);

	meshRenderer->getMesh()->drawInstanced(instanceMatrices, shader->getModelMatrixID());

	meshRenderer->getMesh()->unbind();
	meshRenderer->getTexture()->unbind(0);
}
