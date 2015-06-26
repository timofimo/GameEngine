#include "Camera.h"

/*local includes*/
#include "../Display.h"
#include "../Shaders/Shader.h"
#include "../GameObject.h"

Camera::Camera(float fov, float zNear, float zFar) : ScriptComponent("Camera")
{
	m_fov = fov;
	m_near = zNear;
	m_far = zFar;
	m_ratio = (float)Display::getWidth() / (float)Display::getHeight();

	m_heightNear = 2.0f * tan(m_fov / 2.0f) * m_near;
	m_widthNear = m_heightNear * m_ratio;
	m_heightFar = 2.0f * glm::tan(m_fov / 2.0f) * m_far;
	m_widthFar = m_heightFar * m_ratio;
}


Camera::~Camera()
{
}

void Camera::update(float deltaTime)
{
	glm::vec2 mt = Input::getMouseTranslation();
	/*if (mt != glm::vec2())
		std::cout << "X: " << mt.y * deltaTime << " Y: " << -mt.x * deltaTime << std::endl;*/
	if (glm::abs(mt.x * deltaTime) > 0.1f)
		std::cout << "X: " << mt.x * deltaTime << " MT: " << mt.x << " DT: " << deltaTime << std::endl;
	if (glm::abs(mt.y * deltaTime) > 0.1f)
		std::cout << "Y: " << mt.y * deltaTime << " MT: " << mt.y << " DT: " << deltaTime << std::endl;
	m_parent->getLocalTransform().rotate(glm::vec3(mt.y, -mt.x, 0.0f) * deltaTime);

	float movementSpeed = 10.0f;
	if (Input::getKey(GLFW_KEY_LEFT_SHIFT))
		movementSpeed = 100.0f;
	if (Input::getKey(GLFW_KEY_W))
		m_parent->getLocalTransform().translate(m_parent->getWorldTransform(false).forward() * deltaTime * movementSpeed);
	if (Input::getKey(GLFW_KEY_S))
		m_parent->getLocalTransform().translate(-m_parent->getWorldTransform(false).forward() * deltaTime * movementSpeed);
	if (Input::getKey(GLFW_KEY_D))
		m_parent->getLocalTransform().translate(-m_parent->getWorldTransform(false).right() * deltaTime * movementSpeed);
	if (Input::getKey(GLFW_KEY_A))
		m_parent->getLocalTransform().translate(m_parent->getWorldTransform(false).right() * deltaTime * movementSpeed);

	if (m_parent->transformChanged(false))
		updatePlanes();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(m_parent->getWorldTransform(false).position(), m_parent->getWorldTransform(false).position() + m_parent->getWorldTransform(false).forward(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspectiveFov(m_fov, (float)Display::getWidth(), (float)Display::getHeight(), m_near, m_far);
}

glm::mat4 Camera::getVPMatrix()
{
	return getProjectionMatrix() * getViewMatrix();
}

Transform& Camera::getLocalTransform()
{
	return m_parent->getLocalTransform();
}

bool Camera::pointInFrustum(glm::vec3 point)
{
	for (int i = 0; i < NUM_PLANES; i++)
	{
		if (m_planes[i].getDistance(point) < 0)
			return false;
	}
	return true;
}

bool Camera::sphereInFrustum(glm::vec3 center, float radius)
{
	float distance;
	for (int i = 0; i < NUM_PLANES; i++)
	{
		distance = m_planes[i].getDistance(center);
		if (distance < -radius)
			return false;
	}

	return true;
}

bool Camera::boxInFrustum(glm::vec3 min, glm::vec3 max, glm::quat rotation)
{
	if (rotation != glm::quat())
	{
		glm::vec3 p0 = min, p1 = max;
		p0 = rotation * min;
		p1 = rotation * max;

		if (p0.x < p1.x) p1.x = p0.x;
		else if (p0.x > p1.x) p1.x = p0.x;
		if (p0.y < p1.y) p1.y = p0.y;
		else if (p0.y > p1.y) p1.y = p0.y;
		if (p0.z < p1.z) p1.z = p0.z;
		else if (p0.z > p1.z) p1.z = p0.z;

		min = p1;
		max = p0;
	}

	glm::vec3 p[8];
	p[0] = min;
	p[1] = glm::vec3(min.x, min.y, max.z);
	p[2] = glm::vec3(max.x, min.y, max.z);
	p[3] = glm::vec3(max.x, min.y, min.z);
	p[4] = max;
	p[5] = glm::vec3(max.x, max.y, min.z);
	p[6] = glm::vec3(min.x, max.y, min.z);
	p[7] = glm::vec3(min.x, max.y, max.z);

	for (int i = 0; i < NUM_PLANES; i++)
	{
		int out = 0, in = 0;
		for (int j = 0; j < 8 && (in == 0 || out == 0); j++)
		{
			if (m_planes[i].getDistance(p[j]) < 0)
				out++;
			else
				in++;
		}

		if (in == 0)
			return false;
	}

	return true;
}

void Camera::updatePlanes()
{
	glm::vec3 centerFar = m_parent->getWorldTransform(false).position() + m_parent->getWorldTransform(false).forward() * m_far;
	glm::vec3 ftl = centerFar + (m_parent->getWorldTransform(false).up() * m_heightFar / 2.0f) - (m_parent->getWorldTransform(false).right() * m_widthFar / 2.0f); // far top left
	glm::vec3 ftr = centerFar + (m_parent->getWorldTransform(false).up() * m_heightFar / 2.0f) + (m_parent->getWorldTransform(false).right() * m_widthFar / 2.0f); // far top right
	glm::vec3 fbl = centerFar - (m_parent->getWorldTransform(false).up() * m_heightFar / 2.0f) - (m_parent->getWorldTransform(false).right() * m_widthFar / 2.0f); // far bottom left
	glm::vec3 fbr = centerFar - (m_parent->getWorldTransform(false).up() * m_heightFar / 2.0f) + (m_parent->getWorldTransform(false).right() * m_widthFar / 2.0f); // far bottom right

	glm::vec3 centerNear = m_parent->getWorldTransform(false).position() + m_parent->getWorldTransform(false).forward() * m_near;
	glm::vec3 ntl = centerNear + (m_parent->getWorldTransform(false).up() * m_heightNear / 2.0f) - (m_parent->getWorldTransform(false).right() * m_widthNear / 2.0f); // near top left
	glm::vec3 ntr = centerNear + (m_parent->getWorldTransform(false).up() * m_heightNear / 2.0f) + (m_parent->getWorldTransform(false).right() * m_widthNear / 2.0f); // near top right
	glm::vec3 nbl = centerNear - (m_parent->getWorldTransform(false).up() * m_heightNear / 2.0f) - (m_parent->getWorldTransform(false).right() * m_widthNear / 2.0f); // near bottom left
	glm::vec3 nbr = centerNear - (m_parent->getWorldTransform(false).up() * m_heightNear / 2.0f) + (m_parent->getWorldTransform(false).right() * m_widthNear / 2.0f); // near bottom right

	m_planes[TOP_PLANE].setPoints(ntr, ntl, ftl);
	m_planes[BOTTOM_PLANE].setPoints(nbl, nbr, fbr);
	m_planes[LEFT_PLANE].setPoints(ntl, nbl, fbl);
	m_planes[RIGHT_PLANE].setPoints(nbr, ntr, fbr);
	m_planes[NEAR_PLANE].setPoints(ntl, ntr, nbr);
	m_planes[FAR_PLANE].setPoints(ftr, ftl, fbl);
}
