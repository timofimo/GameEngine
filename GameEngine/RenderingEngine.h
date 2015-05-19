#pragma once

/*local includes*/
#include "GameObject.h"
#include "Shader.h"
#include "Components.h"

class RenderingEngine
{
private:
	std::vector<MeshRenderer*> m_meshRenderers;
	std::vector<LightComponent*> m_lights;

	Display m_display;
	enum Shaders
	{
		SIMPLE_SHADER,
		INSTANCE_SHADER,
		NUM_SHADERS
	}; Shader* m_shaders[NUM_SHADERS];

	Shader* m_activeShader;
	Camera* m_activeCamera;
public:
	RenderingEngine();
	~RenderingEngine();

	void renderScene();

	void setCamera(Camera* cam);
	void setShader(Shader* shader);

	bool windowShouldClose();

	void addMeshRenderer(MeshRenderer* meshRenderer);
	void removeMeshRenderer(MeshRenderer* meshRenderer);

	void addLight(LightComponent* light);
	void removeLight(LightComponent* light);

private:
	void render(MeshRenderer* meshRenderer);
	void renderInstanced(MeshRenderer* meshRenderers);
};

