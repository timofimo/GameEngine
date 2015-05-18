#include "Engine.h"

#include "Components/Script/HelloWorld.h"
#include "Components/Script/ParentScript.h"
#include "Components/Script/Camera.h"
#include "Components/Render/MeshRenderer.h"
#include "ResourceManager.h"

#define FPS 600.0f

Engine::Engine() : root("Root")
{
	//Camera
	GameObject* camera = new GameObject("Camera");
	camera->getLocalTransform().position() = glm::vec3(0.0f, 0.0f, 0.0f);
	camera->getLocalTransform().setRotation(glm::radians(glm::vec3(0.0f, 0.0f, 0.0f)));
	Camera* camComponent = new Camera();
	renderingEngine.setCamera(camComponent);
	camera->addComponent(camComponent);
	root.addChild(camera);

	std::string meshes[] = { "res/chest.obj", "res/coffin.obj", "res/barrel.obj", "res/sack.obj" };
	std::string textures[] = { "res/chest.png", "res/coffin.png", "res/barrel.png", "res/sack.png" };

	for (int i = 0; i < 12000; i++)
	{
		std::string name = "tree" + i;
		GameObject* currentTree = new GameObject(name);
		glm::vec3 position = glm::vec3((float)(rand() % 600), 0.0f, (float)(rand() % 600)) - glm::vec3(300.0f, 0.0f, 300.0f);
		currentTree->getLocalTransform().translate(position);
		currentTree->getLocalTransform().setRotation(glm::vec3(0.0f, (float)(rand() % 360), 0.0f));

		int r = rand() % 4;
		currentTree->addComponent(MeshRenderer::getMeshRenderer(meshes[r], textures[r], &renderingEngine, currentTree));
		root.addChild(currentTree);
	}

	GameObject* terrain = new GameObject("Terrain");
	terrain->getLocalTransform().scale() = glm::vec3(300.0f, 1.0f, 300.0f);
	terrain->addComponent(MeshRenderer::getMeshRenderer("res/plane.obj", "res/grass.png", &renderingEngine, terrain));
	root.addChild(terrain);

	GameObject* dragon = new GameObject("Dragon");
	dragon->getLocalTransform().translate(glm::vec3(-140.0f, 2.0f, 0.0f));
	dragon->getLocalTransform().scale() = glm::vec3(2.0f, 2.0f, 2.0f);
	dragon->getLocalTransform().setRotation(glm::radians(glm::vec3(0.0f, 180.0f, 0.0f)));
	dragon->addComponent(MeshRenderer::getMeshRenderer("res/dragonlp.obj", "res/white.png", &renderingEngine, dragon));
	root.addChild(dragon);

	run();
}


Engine::~Engine()
{
}

void Engine::run()
{
	// INITIALIZE THE SCENE
	root.start();


	float frameStartTime = (float)glfwGetTime();
	float frameTime = 1.0f / FPS;
	float unprocessedTime = 0.0f;

	float fpsTimer = 0.0f;
	int nFrames = 0;

	while (!renderingEngine.windowShouldClose())
	{
		float frameEndTime = (float)glfwGetTime();
		float timePassed = frameEndTime - frameStartTime;
		frameStartTime = frameEndTime;

		unprocessedTime += timePassed;
		fpsTimer += timePassed;

		if (unprocessedTime >= frameTime)
		{
			float startTime = (float)glfwGetTime();
			update(unprocessedTime);
			//std::cout << "Update: " << glfwGetTime() - startTime << std::endl;
			startTime = (float)glfwGetTime();
			render();;
			//std::cout << "Render: " << glfwGetTime() - startTime << std::endl;

			unprocessedTime = 0.0f; // cap to fps limit
			//unprocessedTime -= frameTime; // average on fps limit
			nFrames++;
		}
		if (fpsTimer >= 1.0f)
		{
			std::cout << "FPS: " << nFrames << std::endl;
			nFrames = 0;
			fpsTimer -= 1.0f;
		}
	}
}

void Engine::update(float deltaTime)
{
	glfwPollEvents();
	root.update(deltaTime);
}

void Engine::render()
{
	renderingEngine.renderScene();
}
