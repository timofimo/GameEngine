#include "Engine.h"

#include "Components.h"

#define FPS 60.0f

Engine::Engine() : root("Root")
{
	/*TODO LIST
		+ add soft shadows to spotlights
		+ make soft shadows optional for directional and spotlights
		- make the amount of soft shadow samples taken a uniform
		- add frustum component to camera
		- look for stupid code and remove it
	*/
	std::string meshes[] = { "res/coffin.obj", "res/sack.obj", "res/box.obj", "res/tree.obj", "res/dragonlp.obj", "res/chest.obj", "res/grassPatch.obj" };
	std::string textures[] = { "res/stone.png", "res/grass.png", "res/sack.png", "res/coffin.png", "res/chest.png", "res/grassSprite.png", "res/treeTexture.png" };
	//Camera
	GameObject* camera = new GameObject("Camera");
	camera->getLocalTransform().setPosition(glm::vec3(-3.0f, 1.0f, 0.0f));
	camera->getLocalTransform().setRotation(glm::radians(glm::vec3(0.0f, 0.0f, 0.0f)));
	Camera* camComponent = new Camera();
	renderingEngine.setCamera(camComponent);
	camera->addComponent(camComponent);
	root.addChild(camera);

	GameObject* sphere = new GameObject("sphere");
	sphere->addComponent(MeshRenderer::getMeshRenderer("res/models/sphere.obj", "res/textures/white.png", &renderingEngine, sphere));
	sphere->getLocalTransform().setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	root.addChild(sphere);

	GameObject* sun = new GameObject("sun");
	sun->addComponent(new DirectionalLight("sunLight", glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, glm::vec3(1.0f, -2.0f, 0.0f), &renderingEngine));
	((DirectionalLight*)sun->getComponent("sunLight"))->enableShadowCasting(false, 250.0f);
	root.addChild(sun);

	GameObject* spot = new GameObject("spot");
	spot->addComponent(new SpotLight("spot", glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, -5.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.95f, 0.0f, 0.0f, 0.01f, &renderingEngine));
	((SpotLight*)spot->getComponent("spot"))->enableShadowCasting();
	root.addChild(spot);

	float width = 100.0f;
	float height = 100.0f;
	GameObject* plane = new GameObject("Plane");
	plane->getLocalTransform().setScale(glm::vec3(width, 1.0f, height));
	plane->addComponent(MeshRenderer::getMeshRenderer("res/models/plane.obj", "res/textures/ground.png", &renderingEngine, plane));
	root.addChild(plane);

	for (int i = 0; i < 250; i++)
	{
		float x = glm::sin((float)rand()) * width; 
		float z = glm::cos((float)rand()) * height;
		GameObject* tree = new GameObject("Tree" + i);
		tree->addComponent(MeshRenderer::getMeshRenderer("res/models/tree.obj", "res/textures/treeTexture.png", &renderingEngine, tree));
		tree->getLocalTransform().setPosition(glm::vec3(x, 0.0f, z));
		root.addChild(tree);
	}

	run();
}


Engine::~Engine()
{
}

void Engine::run()
{
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
			render();
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
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		std::cout << error << std::endl;
}
