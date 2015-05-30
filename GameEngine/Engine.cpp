#include "Engine.h"

#include "Components.h"

#define FPS 6000.0f

Engine::Engine() : root("Root")
{
	/*
	The last thing added was the culling for the spot light and the input class.
	The next step is to add culling for spot lights and maybe directional lights (by giving them a range and position).
	After that you can choose between implementing materials/improve obj loader, deferred shading or making standalone light objects( don't delete the components).
	*/
	std::string meshes[] = { "res/coffin.obj", "res/sack.obj", "res/box.obj", "res/tree.obj", "res/dragonlp.obj", "res/chest.obj", "res/grassPatch.obj" };
	std::string textures[] = { "res/stone.png", "res/grass.png", "res/sack.png", "res/coffin.png", "res/chest.png", "res/grassSprite.png", "res/treeTexture.png" };
	//Camera
	GameObject* camera = new GameObject("Camera");
	camera->getLocalTransform().setPosition(glm::vec3(-80.0f, 80.0f, 0.0f));
	camera->getLocalTransform().setRotation(glm::radians(glm::vec3(-45.0f, 0.0f, 0.0f)));
	Camera* camComponent = new Camera();
	renderingEngine.setCamera(camComponent);
	camera->addComponent(camComponent);
	root.addChild(camera);	

	GameObject* plane = new GameObject("Plane");
	plane->getLocalTransform().setScale(glm::vec3(300.0f, 1.0f, 300.0f));
	plane->addComponent(MeshRenderer::getMeshRenderer("res/plane.obj", "res/grass.png", &renderingEngine, plane));
	plane->addComponent(new PointLight("pointLight", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.1f, 0.0f), 0.0f, 0.0f, 0.01f, &renderingEngine));
	root.addChild(plane);
	PointLight* p = (PointLight*)plane->getComponent("pointLight");
	std::cout << p->getRange() << std::endl;

	for (unsigned int i = 0; i < 1500; i++)
	{
		GameObject* tree = new GameObject("tree");
		glm::vec3 position = glm::vec3((float)(rand() % 600), 0.0f, (float)(rand() % 600)) - plane->getLocalTransform().scale();
		tree->getLocalTransform().setPosition(position);
		unsigned int m = rand() % 7;
		unsigned int t = rand() % 7;
		tree->addComponent(MeshRenderer::getMeshRenderer("res/tree.obj", "res/treeTexture.png", &renderingEngine, tree));
		root.addChild(tree);
	}

	GameObject* directionalLight = new GameObject("DirectionalLight");
	directionalLight->addComponent(new DirectionalLight("directionalLight", glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, glm::vec3(0.0f, -10.0f, 1.0f), &renderingEngine));
	root.addChild(directionalLight);

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
	int nFrames = 0, totalFrames = 0, fpsCounted = 1;

	Input::initialize();
	Input::setMouseMode(GLFW_CURSOR_DISABLED);

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
			if (totalFrames == 0)
				totalFrames = nFrames;
			else
			{
				totalFrames += nFrames;
				fpsCounted++;
			}
			std::cout << "FPS: " << nFrames << " AVG: " << totalFrames / fpsCounted << std::endl;
			nFrames = 0;
			fpsTimer -= 1.0f;
		}
	}
}

void Engine::update(float deltaTime)
{
	glfwPollEvents();
	root.update(deltaTime);
	Input::reset();
}

void Engine::render()
{
	renderingEngine.renderScene();
}
