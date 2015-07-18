#include "RenderingEngine.h"

/*local includes*/
#include "Shaders/Shader.h"

#define AMBIENT_COLOR glm::vec3(0.1f, 0.1f, 0.1f)

RenderingEngine::RenderingEngine() : m_display(960, 540, "OpenGL Window", false, false)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// initialize all shaders here
	m_shaders[SIMPLE_SHADER] = new Shader("Simple", "res/shaders/simpleVertexShader.glsl", "res/shaders/simpleFragmentShader.glsl");
	m_shaders[AMBIENT_SHADER] = new Shader("Ambient", "res/shaders/instanceVertexShader.glsl", "res/shaders/ambientFragmentShader.glsl");
	m_shaders[DIRECTIONAL_SHADER] = new Shader("Directional", "res/shaders/instanceVertexShader.glsl", "res/shaders/directionalFragmentShader.glsl");
	m_shaders[POINT_SHADER] = new Shader("Point", "res/shaders/instanceVertexShader.glsl", "res/shaders/pointFragmentShader.glsl");
	m_shaders[SPOT_SHADER] = new Shader("Spot", "res/shaders/instanceVertexShader.glsl", "res/shaders/spotFragmentShader.glsl");
	m_shaders[SHADOWMAP_SHADER] = new Shader("Shadowmap", "res/shaders/shadowMapVertexShader.glsl", "res/shaders/shadowMapFragmentShader.glsl");
	m_shaders[SHADOW_DIRECTIONAL_SHADER] = new Shader("Shadow directional", "res/shaders/shadowVertexShader.glsl", "res/shaders/shadowDirectionalFragmentShader.glsl");
	m_shaders[SHADOW_SPOT_SHADER] = new Shader("Shadow spot", "res/shaders/shadowVertexShader.glsl", "res/shaders/shadowSpotFragmentShader.glsl");

	m_activeShader = m_shaders[AMBIENT_SHADER];

	m_frameBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight(), 2, true);
	for (int i = 0; i < MAX_SHADOWS; i++)
		m_shadowFrameBuffer[i] = new FrameBuffer(4096, 4096, 1, true);
}


RenderingEngine::~RenderingEngine()
{
}

void RenderingEngine::renderScene()
{
	std::vector<LightComponent*> closestLights = getClosestLights(m_activeCamera->getLocalTransform().position());
	int index = 0;
	for each(LightComponent* light in closestLights)
	{
		m_shadowFrameBuffer[index]->bindAsRenderTarget();

		m_activeShader = m_shaders[AMBIENT_SHADER];
		m_activeShader->bind();

		m_activeShader->setUniform("VPMatrix", light->getDepthMVP());
		
		glFrontFace(GL_CW);
		for each (MeshRenderer* mr in m_meshRenderers)
		{
			renderShadowMapInstanced(mr, m_activeShader->getBufferID("ModelMatrix"), light);
		}
		glFrontFace(GL_CCW);

		light->setShadowMapIndex(index);
		index++;
	}

	m_frameBuffer->bindAsRenderTarget();

	if (!m_activeShader || !m_activeCamera)
	{
		std::cout << "ERROR RNEDERINGENGINE: There is no active shader or camera" << std::endl;
		return;
	}

	for each (Shader* s in m_shaders)
	{
		s->bind();
		s->setUniform("VPMatrix", m_activeCamera->getVPMatrix());
	}
	 
	//ambient pass
	m_activeShader = m_shaders[AMBIENT_SHADER];
	m_activeShader->bind();

	m_activeShader->setUniform("ambientColor", AMBIENT_COLOR);
	for each (MeshRenderer* mr in m_meshRenderers)
	{
		renderInstanced(mr, m_activeShader->getBufferID("ModelMatrix"));
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
		if (light->getShadowMapIndex() != -1)
		{
			m_activeShader = m_shaders[SHADOW_DIRECTIONAL_SHADER];
			m_activeShader->bind();
			light->updateUniforms(m_activeShader);
			m_activeShader->setUniform("depthBiasMVP", light->getDepthBias());
			m_activeShader->setSamplerID("shadowMap", 1);
			m_activeShader->setUniform("softShadow", light->castsSoftShadows());
			m_shadowFrameBuffer[light->getShadowMapIndex()]->bindTexture(0, 1);

			for each (MeshRenderer* mr in m_meshRenderers)
			{
				renderInstanced(mr, m_activeShader->getBufferID("ModelMatrix"));
			}

			m_activeShader = m_shaders[DIRECTIONAL_SHADER];
			m_activeShader->bind();
			light->setShadowMapIndex(-1);
		}
		else
		{
			light->updateUniforms(m_activeShader);

			for each (MeshRenderer* mr in m_meshRenderers)
			{
				renderInstanced(mr, m_activeShader->getBufferID("ModelMatrix"));
			}
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
			renderInstanced(mr, m_activeShader->getBufferID("ModelMatrix"), light);
		}
	}	

	//spot pass
	m_activeShader = m_shaders[SPOT_SHADER];
	m_activeShader->bind();
	for each (SpotLight* light in m_spotLights)
	{
		if (light->getShadowMapIndex() != -1)
		{
			m_activeShader = m_shaders[SHADOW_SPOT_SHADER];
			m_activeShader->bind();
			light->updateUniforms(m_activeShader);
			m_activeShader->setUniform("depthBiasMVP", light->getDepthBias());
			m_activeShader->setSamplerID("shadowMap", 1);
			m_activeShader->setUniform("softShadow", light->castsSoftShadows());
			m_shadowFrameBuffer[light->getShadowMapIndex()]->bindTexture(0, 1);

			for each (MeshRenderer* mr in m_meshRenderers)
			{
				renderInstanced(mr, m_activeShader->getBufferID("ModelMatrix"));
			}

			m_activeShader = m_shaders[SPOT_SHADER];
			m_activeShader->bind();
			light->setShadowMapIndex(-1);
		}
		else
		{
			light->updateUniforms(m_activeShader);

			for each (MeshRenderer* mr in m_meshRenderers)
			{
				renderInstanced(mr, m_activeShader->getBufferID("ModelMatrix"), light);
			}
		}
	}
	
	//reset options
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	drawPlane();

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

Camera* RenderingEngine::getCamera()
{
	return m_activeCamera;
}

float RenderingEngine::getAmbientIntensity()
{
	return glm::max(AMBIENT_COLOR.r, glm::max(AMBIENT_COLOR.g, AMBIENT_COLOR.b));
}

void RenderingEngine::renderInstanced(MeshRenderer* meshRenderer, GLuint ModelMatrixID, PointLight* rangedLight/* = nullptr*/)
{
	//double startTime = glfwGetTime();
	std::vector<glm::mat4> instanceMatrices;	
	for each (GameObject* parent in meshRenderer->getParents())
	{
		Transform t = parent->getWorldTransform(false);

		if (m_activeCamera->sphereInFrustum(t.position() + meshRenderer->getMesh()->getBoundingSphereCenter() * t.scale(), meshRenderer->getMesh()->getRadius() * t.scalef()))
		{
			if (rangedLight != nullptr)
			{
				float distance = glm::distance(t.position() + meshRenderer->getMesh()->getBoundingSphereCenter() * t.scale(), rangedLight->getPosition());
				float meshRadius = meshRenderer->getMesh()->getRadius() * t.scalef();

				if (distance <= rangedLight->getRange(0.01f) + meshRadius)
					instanceMatrices.push_back(parent->getWorldTransform(true).modelMatrix());
			}
			else
				instanceMatrices.push_back(parent->getWorldTransform(true).modelMatrix());
		}
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


void RenderingEngine::renderShadowMapInstanced(MeshRenderer* meshRenderer, GLuint ModelMatrixID, LightComponent* light)
{
	//double startTime = glfwGetTime();

	PhysicsComponent* cullingObject = 0;

	if (light->getType() == LightComponent::DIRECTIONAL_LIGHT)
		cullingObject = ((DirectionalLight*)light)->getCullingObject();
	else if (light->getType() == LightComponent::SPOT_LIGHT)
		cullingObject = ((SpotLight*)light)->getCullingObject();

	std::vector<glm::mat4> instanceMatrices;
	for each (GameObject* parent in meshRenderer->getParents())
	{
		Transform t = parent->getWorldTransform(false);

		SphereComponent boundingSphere = SphereComponent(meshRenderer->getMesh()->getBoundingSphereCenter() + t.position(), meshRenderer->getMesh()->getRadius() * t.scalef());
		if (cullingObject->checkCollision(&boundingSphere))
		{
			instanceMatrices.push_back(parent->getWorldTransform(false).modelMatrix());
		}
	}
	//std::cout << meshRenderer->getName().c_str() << ": " << glfwGetTime() - startTime << std::endl;
	std::cout << light->getType() << ": " << instanceMatrices.size() << std::endl;
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
	unsigned int maxShadowMappedLights = 3;
	std::vector<LightComponent*> closestLights;
	for each (LightComponent* l in m_directionalLights)
	{
		if (l->isShadowCaster())
		{
			closestLights.push_back(l);
			if (closestLights.size() >= maxShadowMappedLights)
				return closestLights;
		}
	}

	for each (SpotLight* l in m_spotLights)
	{
		if (l->isShadowCaster())
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
	}

	return closestLights;
}

void RenderingEngine::drawPlane()
{
	Display::bindAsRenderTarget();

	m_activeShader = m_shaders[SIMPLE_SHADER];
	m_activeShader->bind();

	if (Input::getKey(GLFW_KEY_Z))
		m_shadowFrameBuffer[0]->bindTexture(0, 0);
	else if (Input::getKey(GLFW_KEY_X))
		m_frameBuffer->bindTexture(1, 0);
	else
		m_frameBuffer->bindTexture(0, 0);

	Mesh* m = Mesh::getMesh("res/models/1x1planer.obj");
	m->bind();
	m->draw();
}