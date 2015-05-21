#include "Engine.h"

#include "Components.h"

#define FPS 6000.0f

Engine::Engine() : root("Root")
{
	//Camera
	GameObject* camera = new GameObject("Camera");
	camera->getLocalTransform().setPosition(glm::vec3(0.0f, 10.0f, 0.0f));
	camera->getLocalTransform().setRotation(glm::radians(glm::vec3(0.0f, 0.0f, 0.0f)));
	Camera* camComponent = new Camera();
	renderingEngine.setCamera(camComponent);
	camera->addComponent(camComponent);
	root.addChild(camera);

	unsigned int amount = 15;
	for (unsigned int x = 0; x < amount; x++)
	{
		for (unsigned int z = 0; z < amount; z++)
		{
			GameObject* monkey = new GameObject("Monkey");
			monkey->getLocalTransform().translate(glm::vec3(x * 3.0f, 0.0f, z * 3.0f));
			monkey->getLocalTransform().rotate(glm::radians(glm::vec3(0.0f, -90.0f, 0.0f)));
			monkey->addComponent(MeshRenderer::getMeshRenderer("res/monkey.obj", "res/white.png", &renderingEngine, monkey));
			root.addChild(monkey);
		}
	}

	for (int i = 0; i < 3; i++)
	{
		GameObject* lights = new GameObject("Lights" + i);
		lights->getLocalTransform().setPosition(glm::vec3(0.0f, 1.0f, (float)((i) * amount)));
		lights->addComponent(new PointLight("PointLight", glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.1f, &renderingEngine));
		lights->addComponent(new Movement());
		root.addChild(lights);
	}

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
