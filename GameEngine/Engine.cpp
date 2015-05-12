#include "Engine.h"

#include "Components/Script/HelloWorld.h"
#include "Components/Script/ParentScript.h"
#include "Components/Script/Camera.h"
#include "Components/Render/MeshRenderer.h"

#define FPS 60.0f

Engine::Engine() : m_mainDisplay(640, 360, "OpenGL Window", false, false), m_shader("res/vertexShader.glsl", "res/fragmentShader.glsl"), root("Root")
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	m_shader.bind();

	//Camera
	GameObject* camera = new GameObject("Camera");
	camera->getLocalTransform().position() = glm::vec3(0.0f, 0.0f, -50.0f);
	camera->getLocalTransform().setRotation(glm::radians(glm::vec3(0.0f, 90.0f, 0.0f)));
	Camera* camComponent = new Camera();
	camComponent->setActive();
	camera->addComponent(camComponent);
	root.addChild(camera);

	/*//Box
	GameObject* box = new GameObject("Box");
	box->getLocalTransform().translate(glm::vec3(0.0f, 0.0f, 0.0f));
	box->addComponent(new MeshRenderer("BoxRenderer", "res/box.obj", "res/stone.png"));
	root.addChild(box);

	GameObject* box2 = new GameObject("Box2");
	box2->getLocalTransform().translate(glm::vec3(1.0f, 2.0f, 0.0f));
	box2->getLocalTransform().resize(glm::vec3(0.5f, 0.5f, 0.5f));
	box2->addComponent(new MeshRenderer("BoxRenderer", "res/box.obj", "res/stone.png"));
	box->addComponent(new ParentScript());
	box->addChild(box2);*/

	GameObject* lastBox = nullptr;
	for (int i = 0; i < 100; i++)
	{
		if (lastBox == nullptr)
		{
			std::string name = "Box" + (i - 1);
			lastBox = new GameObject(name);
			lastBox->addComponent(new MeshRenderer("BoxRenderer", "res/box.obj", "res/stone.png"));
			lastBox->addComponent(new ParentScript());
			root.addChild(lastBox);
		}

		std::string name = "Box" + i;
		GameObject* currentBox = new GameObject(name);
		currentBox->getLocalTransform().translate(glm::vec3(2.0f, 2.0f, 0.0f));
		currentBox->getLocalTransform().resize(glm::vec3(0.99f, 0.99f, 0.99f));
		currentBox->addComponent(new MeshRenderer("BoxRenderer", "res/box.obj", "res/stone.png"));
		currentBox->addComponent(new ParentScript());
		lastBox->addChild(currentBox);
		lastBox = currentBox;
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
