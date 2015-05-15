#include "Camera.h"

/*local includes*/
#include "../Display.h"
#include "../Shader.h"
#include "../GameObject.h"

Camera::Camera() : ScriptComponent("Camera")
{
}


Camera::~Camera()
{
}

void Camera::update(float deltaTime)
{
	if (glfwGetKey(Display::getWindow(), GLFW_KEY_UP))
		m_parent->getLocalTransform().rotate(glm::vec3(1.0f, 0.0f, 0.0f) * deltaTime);
	if (glfwGetKey(Display::getWindow(), GLFW_KEY_DOWN))
		m_parent->getLocalTransform().rotate(glm::vec3(-1.0f, 0.0f, 0.0f) * deltaTime);
	if (glfwGetKey(Display::getWindow(), GLFW_KEY_LEFT))
		m_parent->getLocalTransform().rotate(glm::vec3(0.0f, -1.0f, 0.0f) * deltaTime);
	if (glfwGetKey(Display::getWindow(), GLFW_KEY_RIGHT))
		m_parent->getLocalTransform().rotate(glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime);

	float movementSpeed = 10.0f;
	if (glfwGetKey(Display::getWindow(), GLFW_KEY_W))
		m_parent->getLocalTransform().translate(m_parent->getWorldTransform().forward() * deltaTime * movementSpeed);
	if (glfwGetKey(Display::getWindow(), GLFW_KEY_S))
		m_parent->getLocalTransform().translate(-m_parent->getWorldTransform().forward() * deltaTime * movementSpeed);
	if (glfwGetKey(Display::getWindow(), GLFW_KEY_D))
		m_parent->getLocalTransform().translate(-m_parent->getWorldTransform().right() * deltaTime * movementSpeed);
	if (glfwGetKey(Display::getWindow(), GLFW_KEY_A))
		m_parent->getLocalTransform().translate(m_parent->getWorldTransform().right() * deltaTime * movementSpeed);
}

void Camera::setActive()
{
	Shader::setActiveCamera(this);
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(m_parent->getWorldTransform().position(), m_parent->getWorldTransform().position() + m_parent->getWorldTransform().forward(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspectiveFov(70.0f, (float)Display::getWidth(), (float)Display::getHeight(), 0.1f, 1000.0f);
}

glm::mat4 Camera::getVPMatrix()
{
	return getProjectionMatrix() * getViewMatrix();
}
