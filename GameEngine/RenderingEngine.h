#pragma once

#define MAX_SHADOWS 3

/*local includes*/
#include "GameObjects/GameObject.h"
#include "Shaders/Shader.h"
#include "Components.h"
#include "FrameBuffer.h"

class RenderingEngine
{
private:
	std::vector<MeshRenderer*> m_meshRenderers;
	std::vector<DirectionalLightComponent*> m_directionalLights;
	std::vector<PointLightComponent*> m_pointLights;
	std::vector<SpotLightComponent*> m_spotLights;

	Display m_display;
	enum Shaders
	{
		SIMPLE_SHADER,
		AMBIENT_SHADER,
		DIRECTIONAL_SHADER,
		POINT_SHADER,
		SPOT_SHADER,
		SHADOWMAP_SHADER,
		SHADOW_DIRECTIONAL_SHADER,
		SHADOW_SPOT_SHADER,
		NUM_SHADERS
	}; Shader* m_shaders[NUM_SHADERS];

	Shader* m_activeShader;
	Camera* m_activeCamera;
	FrameBuffer* m_frameBuffer, *m_shadowFrameBuffer[MAX_SHADOWS];
public:
	RenderingEngine();
	~RenderingEngine();

	static RenderingEngine& get();

	void renderScene();

	void setCamera(Camera* cam);
	void setShader(Shader* shader);

	bool windowShouldClose();

	void addMeshRenderer(MeshRenderer* meshRenderer);
	void removeMeshRenderer(MeshRenderer* meshRenderer);

	void addLight(LightComponent* light);
	void removeLight(LightComponent* light);

	Camera* getCamera();
	float getAmbientIntensity();

private:
	void renderInstanced(MeshRenderer* meshRenderer, GLuint ModelMatrixID, LightComponent* rangedLight = nullptr);
	void renderShadowMapInstanced(MeshRenderer* meshRenderer, GLuint ModelMatrixID, LightComponent* light);

	std::vector<LightComponent*> getClosestLights(glm::vec3 point);
	void drawPlane();
};

