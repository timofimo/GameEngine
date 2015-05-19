#include "RenderingEngine.h"

/*local includes*/
#include "SimpleShader.h"
#include "InstanceShader.h"

#define AMBIENT_COLOR glm::vec3(0.3f, 0.3f, 0.3f)
#define LIGHT_DIRECTION glm::vec3(0.1f, -0.5f, 0.0f)
#define LIGHT_COLOR glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)

RenderingEngine::RenderingEngine() : m_display(640, 360, "OpenGL Window", false, false)
{
	glClearColor(0.5f, 0.8f, 1.0f, 1.0f);

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
		std::cout << "ERROR RNEDERINGENGINE: There is no active shader or camera" << std::endl;
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

void RenderingEngine::removeMeshRenderer(MeshRenderer* meshRenderer)
{
	std::vector<MeshRenderer*>::iterator it = std::find(m_meshRenderers.begin(), m_meshRenderers.end(), meshRenderer);
	if (it == m_meshRenderers.end())
		std::cout << "ERROR RENDERINGENGINE: tried to delete a meshrenderer that doesn't exist" << std::endl;
	else
		m_meshRenderers.erase(it);
}

void RenderingEngine::addLight(LightComponent* light)
{
	m_lights.push_back(light);
}

void RenderingEngine::removeLight(LightComponent* light)
{
	std::vector<LightComponent*>::iterator it = std::find(m_lights.begin(), m_lights.end(), light);
	if (it == m_lights.end())
		std::cout << "ERROR RENDERINGENGINE: tried to delete a light that doesn't exist" << std::endl;
	else
		m_lights.erase(it);
}

void RenderingEngine::render(MeshRenderer* meshRenderer)
{
	SimpleShader* shader = (SimpleShader*)m_shaders[SIMPLE_SHADER];
	shader->bind();

	shader->setAmbientColor(AMBIENT_COLOR);
	shader->setEyePosition(m_activeCamera->getPosition());
	shader->setLightDirection(LIGHT_DIRECTION);
	shader->setLightColor(LIGHT_COLOR);
	shader->setSpecularIntensity(0.1f);
	shader->setSpecularPower(8.0f);

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

	shader->setAmbientColor(AMBIENT_COLOR);
	shader->setEyePosition(m_activeCamera->getPosition());
	shader->setLightDirection(LIGHT_DIRECTION);
	shader->setLightColor(LIGHT_COLOR);
	shader->setSpecularIntensity(0.1f);
	shader->setSpecularPower(8.0f);

	std::vector<glm::mat4> instanceMatrices;
	if (meshRenderer->parentsTransformChanged())
	{
		for each (GameObject* parent in meshRenderer->getParents())
		{
			instanceMatrices.push_back(parent->getWorldTransform().modelMatrix());
		}
	}

	meshRenderer->getMesh()->bind();
	meshRenderer->getTexture()->bind(0);

	meshRenderer->getMesh()->drawInstanced(instanceMatrices, shader->getModelMatrixID());

	meshRenderer->getMesh()->unbind();
	meshRenderer->getTexture()->unbind(0);
}
