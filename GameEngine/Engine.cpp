#include "Engine.h"

#include "Components.h"

#define FPS 60.0f

Engine::Engine() : root("Root")
{
	/*
	The last thing added was the culling for the point lights and the input class.
	The next step is to add culling for spot lights and maybe directional lights (by giving them a range and position).
	After that you can choose between implementing materials/improve obj loader, deferred shading or making standalone light objects( don't delete the components).
	*/

	std::string meshes[] = { "res/coffin.obj", "res/sack.obj", "res/box.obj", "res/tree.obj", "res/dragonlp.obj", "res/chest.obj", "res/grassPatch.obj" };
	std::string textures[] = { "res/stone.png", "res/grass.png", "res/sack.png", "res/coffin.png", "res/chest.png", "res/grassSprite.png", "res/treeTexture.png" };
	//Camera
	GameObject* camera = new GameObject("Camera");
	camera->getLocalTransform().setPosition(glm::vec3(-50.0f, 50.0f, 0.0f));
	camera->getLocalTransform().setRotation(glm::radians(glm::vec3(-45.0f, 0.0f, 0.0f)));
	Camera* camComponent = new Camera();
	renderingEngine.setCamera(camComponent);
	camera->addComponent(camComponent);
	root.addChild(camera);	

	GameObject* plane = new GameObject("Plane");
	plane->getLocalTransform().setScale(glm::vec3(25.0f, 1.0f, 25.0f));
	plane->addComponent(MeshRenderer::getMeshRenderer("res/plane.obj", "res/white.png", &renderingEngine, plane));
	root.addChild(plane);

	GameObject* light = new GameObject("light");
	light->addComponent(new PointLight("pointlight", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.5f, &renderingEngine));
	light->addComponent(new LightMovementScript());
	root.addChild(light);

	GameObject* light2 = new GameObject("light2");
	light2->addComponent(new DirectionalLight("directionalLight", glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, glm::vec3(0.5f, -1.0f, 0.0f), &renderingEngine));
	root.addChild(light2);

	GameObject* light3 = new GameObject("light3");
	light3->addComponent(new PointLight("pointlight2", glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(10.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.5f, &renderingEngine));
	light3->addComponent(new LightMovementScript());
	root.addChild(light3);

	GameObject* light4 = new GameObject("light4");
	light4->addComponent(new PointLight("pointlight3", glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-10.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.5f, &renderingEngine));
	light4->addComponent(new LightMovementScript());
	root.addChild(light4);

	/*GameObject* sphere = new GameObject("sphere");
	sphere->getLocalTransform().translate(glm::vec3(15.0f, 1.0f, 0.0f));
	sphere->addComponent(MeshRenderer::getMeshRenderer("res/dragonlp.obj", "res/white.png", &renderingEngine, sphere));
	//sphere->addComponent(new ParentScript());
	root.addChild(sphere);*/

	/*for (int i = 0; i <= 1; i++)
	{
		GameObject* light = new GameObject("light");
		light->addComponent(new PointLight("pointlight", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(i * 5.0f - 25.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.5f, &renderingEngine));
		light->addComponent(new ParentScript());
		root.addChild(light);
	}

	for (int i = 0; i <= 1; i++)
	{
		GameObject* light = new GameObject("light");
		light->addComponent(new SpotLight("spotlight", glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, i * 5.0f - 25.0f), glm::vec3(1.0f, -1.0f, 0.0f), 0.7f, 0.0f, 0.0f, 0.5f, &renderingEngine));
		light->addComponent(new ParentScript());
		root.addChild(light);
	}*/

	for (int x = -25; x <= 25; x += 5)
	{
		for (int z = -25; z <= 25; z += 5)
		{
			GameObject* sphere = new GameObject("sphere");
			sphere->getLocalTransform().translate(glm::vec3((float)x, 1.0f, (float)z));
			sphere->addComponent(MeshRenderer::getMeshRenderer("res/sphere.obj", "res/white.png", &renderingEngine, sphere));
			root.addChild(sphere);
		}
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
