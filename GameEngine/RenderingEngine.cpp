#include "RenderingEngine.h"

/*local includes*/
#include "Shaders/SimpleShader.h"
#include "Shaders/InstanceShader.h"
#include "Shaders/InstanceAmbientShader.h"
#include "Shaders/InstanceDirectionalShader.h"
#include "Shaders/InstanceDirectionalShadowShader.h"
#include "Shaders/InstancePointShader.h"
#include "Shaders/InstanceSpotShader.h"

#define AMBIENT_COLOR glm::vec3(0.1f, 0.1f, 0.1f)

RenderingEngine::RenderingEngine() : m_display(960, 540, "OpenGL Window", false, false)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// initialize all shaders here
	m_shaders[AMBIENT_SHADER] = new InstanceAmbientShader();
	m_shaders[DIRECTIONAL_SHADER] = new InstanceDirectionalShader();
	m_shaders[DIRECTIONAL_SHADOW_SHADER] = new InstanceDirectionalShadowShader();
	m_shaders[POINT_SHADER] = new InstancePointShader();
	m_shaders[SPOT_SHADER] = new InstanceSpotShader();

	m_activeShader = m_shaders[AMBIENT_SHADER];
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

	//ambient pass
	m_activeShader = m_shaders[AMBIENT_SHADER];
	m_activeShader->bind();

	((InstanceAmbientShader*)m_activeShader)->setAmbientColor(AMBIENT_COLOR);
	for each (MeshRenderer* mr in m_meshRenderers)
	{
		renderInstanced(mr, ((InstanceAmbientShader*)m_activeShader)->getModelMatrixID());
	}

	//set options
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	//directional pass
	m_activeShader = m_shaders[DIRECTIONAL_SHADER];
	m_activeShader->bind();
	for each (DirectionalLight* light in m_directionalLights)
	{
		light->updateUniforms(m_activeShader);

		for each (MeshRenderer* mr in m_meshRenderers)
		{
			renderInstanced(mr, ((InstanceDirectionalShader*)m_activeShader)->getModelMatrixID());
		}
	}	

	//point pass
	m_activeShader = m_shaders[POINT_SHADER];
	m_activeShader->bind();
	
	for each (PointLight* light in m_pointLights)
	{
		light->updateUniforms(m_activeShader);

		for each (MeshRenderer* mr in m_meshRenderers)
		{
			renderInstancedRangedLights(mr, ((InstancePointShader*)m_activeShader)->getModelMatrixID(), light->getPosition(), light->getRange(0.01f));
		}
	}	

	//spot pass
	m_activeShader = m_shaders[SPOT_SHADER];
	m_activeShader->bind();
	for each (SpotLight* light in m_spotLights)
	{
		light->updateUniforms(m_activeShader);

		for each (MeshRenderer* mr in m_meshRenderers)
		{
			renderInstancedRangedLights(mr, ((InstanceSpotShader*)m_activeShader)->getModelMatrixID(), light->getPosition(), light->getRange());
		}
	}
	
	//reset options
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

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
	LightComponent::LightTypes type = light->getType();
	switch (type)
	{
	case LightComponent::DIRECTIONAL_LIGHT:
		m_directionalLights.push_back((DirectionalLight*)light);
		break;
	case LightComponent::SPOT_LIGHT:
		m_spotLights.push_back((SpotLight*)light);
		break;
	case LightComponent::POINT_LIGHT:
		m_pointLights.push_back((SpotLight*)light);
		break;
	default:
		break;
	}
}

void RenderingEngine::removeLight(LightComponent* light)
{
	LightComponent::LightTypes type = light->getType();
	if (type == LightComponent::DIRECTIONAL_LIGHT)
	{
		std::vector<DirectionalLight*>::iterator it = std::find(m_directionalLights.begin(), m_directionalLights.end(), light);
		if (it == m_directionalLights.end())
			std::cout << "ERROR RENDERINGENGINE: tried to delete a light that doesn't exist" << std::endl;
		else
			m_directionalLights.erase(it);
	}
	else if (type == LightComponent::POINT_LIGHT)
	{
		std::vector<PointLight*>::iterator it = std::find(m_pointLights.begin(), m_pointLights.end(), light);
		if (it == m_pointLights.end())
			std::cout << "ERROR RENDERINGENGINE: tried to delete a light that doesn't exist" << std::endl;
		else
			m_pointLights.erase(it);
	}
	else if (type == LightComponent::SPOT_LIGHT)
	{
		std::vector<SpotLight*>::iterator it = std::find(m_spotLights.begin(), m_spotLights.end(), light);
		if (it == m_spotLights.end())
			std::cout << "ERROR RENDERINGENGINE: tried to delete a light that doesn't exist" << std::endl;
		else
			m_spotLights.erase(it);
	}	
}

void RenderingEngine::render(MeshRenderer* meshRenderer, Shader* shader)
{
	/*meshRenderer->getMesh()->bind();
	meshRenderer->getTexture()->bind(0);

	for each (GameObject* parent in meshRenderer->getParents())
	{
		shader->UpdateModelMatrix(parent->getWorldTransform().modelMatrix());
		meshRenderer->getMesh()->draw();
	}

	meshRenderer->getMesh()->unbind();
	meshRenderer->getTexture()->unbind(0);*/
}

void RenderingEngine::renderInstanced(MeshRenderer* meshRenderer, GLuint ModelMatrixID)
{
	//double startTime = glfwGetTime();
	/*std::vector<glm::mat4> instanceMatrices;
	if (meshRenderer->parentsTransformChanged())
	{
		for each (GameObject* parent in meshRenderer->getParents())						//old way - without frustum culling
		{
			instanceMatrices.push_back(parent->getWorldTransform(true).modelMatrix());
		}
	}*/
	std::vector<glm::mat4> instanceMatrices;	
	for each (GameObject* parent in meshRenderer->getParents())
	{
		Transform t = parent->getWorldTransform(false);

		if (m_activeCamera->sphereInFrustum(t.position() + meshRenderer->getMesh()->getBoundingSphereCenter() * t.scale(), meshRenderer->getMesh()->getRadius() * t.scalef()))
			instanceMatrices.push_back(parent->getWorldTransform(true).modelMatrix());
	}
	//std::cout << meshRenderer->getName().c_str() << ": " << glfwGetTime() - startTime << std::endl;

	if (instanceMatrices.size() > 0)
	{
		meshRenderer->getMesh()->bind();
		meshRenderer->getTexture()->bind(0);


		meshRenderer->getMesh()->drawInstanced(instanceMatrices, ModelMatrixID);

		meshRenderer->getMesh()->unbind();
		meshRenderer->getTexture()->unbind(0);
	}
}

void RenderingEngine::renderInstancedRangedLights(MeshRenderer* meshRenderer, GLuint ModelMatrixID, glm::vec3 position, float range)
{
	std::vector<glm::mat4> instanceMatrices;
	for each (GameObject* parent in meshRenderer->getParents())
	{
		Transform t = parent->getWorldTransform(false);

		if (m_activeCamera->sphereInFrustum(t.position() + meshRenderer->getMesh()->getBoundingSphereCenter() * t.scale(), meshRenderer->getMesh()->getRadius() * t.scalef()))
		{
			float distance = glm::distance(t.position() + meshRenderer->getMesh()->getBoundingSphereCenter() * t.scale(), position);
			float meshRadius = meshRenderer->getMesh()->getRadius() * t.scalef();

			if (distance <= range + meshRadius)
				instanceMatrices.push_back(parent->getWorldTransform(true).modelMatrix());
		}
	}

	if (instanceMatrices.size() > 0)
	{
		meshRenderer->getMesh()->bind();
		meshRenderer->getTexture()->bind(0);


		meshRenderer->getMesh()->drawInstanced(instanceMatrices, ModelMatrixID);

		meshRenderer->getMesh()->unbind();
		meshRenderer->getTexture()->unbind(0);
	}
}

std::vector<LightComponent*> RenderingEngine::getClosestLights(glm::vec3 point)
{
	int maxShadowMappedLights = 3;
	std::vector<LightComponent*> closestLights;
	for each (LightComponent* l in m_directionalLights)
	{
		closestLights.push_back(l);
		if (closestLights.size() >= maxShadowMappedLights)
			return closestLights;
	}

	for each (SpotLight* l in m_spotLights)
	{
		int index = 0;
		float distance = glm::distance(l->getPosition(), point);
		bool lightAdded = false;
		for each (LightComponent* cl in closestLights)
		{
			float distance2 = cl->getType() == LightComponent::DIRECTIONAL_LIGHT ? 0.0f : glm::distance(((SpotLight*)cl)->getPosition(), point);
			if (distance < distance2)
			{
				closestLights.insert(closestLights.begin() + index, l);
				lightAdded = true;
				break;
			}
			index++;
		}

		if (!lightAdded && closestLights.size() < maxShadowMappedLights)
		{
			closestLights.push_back(l);
			lightAdded = true;
		}

		if (lightAdded && closestLights.size() >= maxShadowMappedLights)
			closestLights.resize(maxShadowMappedLights);
	}

	for each (PointLight* l in m_pointLights)
	{
		int index = 0;
		float distance = glm::distance(l->getPosition(), point);
		bool lightAdded = false;
		for each (LightComponent* cl in closestLights)
		{
			float distance2 = cl->getType() == LightComponent::DIRECTIONAL_LIGHT ? 0.0f : glm::distance(((SpotLight*)cl)->getPosition(), point);
			if (distance < distance2)
			{
				closestLights.insert(closestLights.begin() + index, l);
				lightAdded = true;
				break;
			}
			index++;
		}

		if (!lightAdded && closestLights.size() < maxShadowMappedLights)
		{
			closestLights.push_back(l);
			lightAdded = true;
		}

		if (lightAdded && closestLights.size() >= maxShadowMappedLights)
			closestLights.resize(maxShadowMappedLights);
	}

	return closestLights;
}

void RenderingEngine::renderDirectionalShadowMap(DirectionalLight* l)
{
	GLuint frameBuffer = 0;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "RENDERINGENGINE: getShadowMap" << std::endl;
		return;
	}

	//Light matrices
	m_activeShader = m_shaders[DIRECTIONAL_SHADOW_SHADER];
	m_activeShader->bind();
	l->updateShadowUniforms(m_activeShader, m_activeCamera->getLocalTransform().position());

	//Render the scene
	for each(MeshRenderer* meshRenderer in m_meshRenderers)
	{
		std::vector<glm::mat4> instanceMatrices;
		if (meshRenderer->parentsTransformChanged())
		{
			for each (GameObject* parent in meshRenderer->getParents())	
			{
				instanceMatrices.push_back(parent->getWorldTransform(true).modelMatrix());
			}
		}

		if (instanceMatrices.size() > 0)
		{
			meshRenderer->getMesh()->bind();

			meshRenderer->getMesh()->drawInstanced(instanceMatrices, ((InstanceDirectionalShadowShader*)m_activeShader)->getModelMatrixID());

			meshRenderer->getMesh()->unbind();
		}
	}

	l->setShadowMap(depthTexture);
}

void RenderingEngine::renderSpotShadowMap(SpotLight* l)
{
	std::cout << l->getName().c_str() << std::endl;
}

void RenderingEngine::renderPointShadowMap(PointLight* l)
{
	std::cout << l->getName().c_str() << std::endl;
}


