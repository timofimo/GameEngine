#include "Engine.h"

#include "Components/Script/HelloWorld.h"
#include "Components/Script/ParentScript.h"
#include "Components/Script/Camera.h"
#include "Components/Render/MeshRenderer.h"
#include "ResourceManager.h"

#define FPS 60.0f

Engine::Engine() : m_mainDisplay(640, 360, "OpenGL Window", false, false), m_shader("res/vertexShader.glsl", "res/fragmentShader.glsl"), root("Root")
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	m_shader.bind();

	//Camera
	GameObject* camera = new GameObject("Camera");
	camera->getLocalTransform().position() = glm::vec3(-150.0f, 15.0f, 0.0f);
	camera->getLocalTransform().setRotation(glm::radians(glm::vec3(0.0f, 00.0f, 0.0f)));
	Camera* camComponent = new Camera();
	camComponent->setActive();
	camera->addComponent(camComponent);
	root.addChild(camera);

	/*int amount = 20;
	for (int x = 0; x < amount; x++)
	{
		for (int z = 0; z < amount; z++)
		{
			std::string name = "Box" + x + z;
			GameObject* currentBox = new GameObject(name);
			currentBox->getLocalTransform().translate(glm::vec3(x * 3.0f, 0.0f, z * 3.0f));

			currentBox->addComponent(new MeshRenderer("BoxRenderer", "res/tree.obj", "res/treeTexture.png"));
			currentBox->addComponent(new ParentScript());
			root.addChild(currentBox);
		}
	}*/

	for (int i = 0; i < 600; i++)
	{
		std::string name = "tree" + i;
		GameObject* currentTree = new GameObject(name);
		glm::vec3 position = glm::vec3((float)(rand() % 300), 0.0f, (float)(rand() % 300)) - glm::vec3(150.0f, 0.0f, 150.0f);
		currentTree->getLocalTransform().translate(position);
		currentTree->getLocalTransform().setRotation(glm::vec3(0.0f, (float)(rand() % 360), 0.0f));

		currentTree->addComponent(new MeshRenderer("TreeRenderer", "res/tree.obj", "res/treeTexture.png"));
		root.addChild(currentTree);
	}

	GameObject* terrain = new GameObject("Terrain");
	terrain->getLocalTransform().scale() = glm::vec3(150.0f, 1.0f, 150.0f);
	terrain->addComponent(new MeshRenderer("TerrainRenderer", "res/plane.obj", "res/grass.png"));
	root.addChild(terrain);

	GameObject* dragon = new GameObject("Dragon");
	dragon->getLocalTransform().translate(glm::vec3(-140.0f, 2.0f, 0.0f));
	dragon->getLocalTransform().scale() = glm::vec3(2.0f, 2.0f, 2.0f);
	dragon->getLocalTransform().setRotation(glm::radians(glm::vec3(0.0f, 180.0f, 0.0f)));
	dragon->addComponent(new MeshRenderer("DragonRenderer", "res/dragonlp.obj", "res/white.png"));
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

	while (!m_mainDisplay.shouldClose())
	{
		float frameEndTime = (float)glfwGetTime();
		float timePassed = frameEndTime - frameStartTime;
		frameStartTime = frameEndTime;

		unprocessedTime += timePassed;
		fpsTimer += timePassed;

		if (unprocessedTime >= frameTime)
		{
			update(unprocessedTime);
			render();

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_shader.updateCamera();
	root.render(&m_shader);
	glfwSwapBuffers(m_mainDisplay.getWindow());
}
