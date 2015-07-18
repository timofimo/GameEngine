#include "FrustumComponent.h"

/*local includes*/
#include "../Display.h"
#include "../GameObject.h"

FrustumComponent::FrustumComponent(float left, float right, float bottom, float top, float zNear, float zFar) : PhysicsComponent(PhysicsComponent::FRUSTUM_COMPONENT, "frustumComponent")
{
	m_near = zNear;
	m_far = zFar;

	m_heightNear = m_heightFar = top - bottom;
	m_widthNear = m_widthFar = right - left;
}

FrustumComponent::FrustumComponent(float fov, float zNear, float zFar) : PhysicsComponent(PhysicsComponent::FRUSTUM_COMPONENT, "frustumComponent")
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

FrustumComponent::~FrustumComponent()
{
}

bool FrustumComponent::checkCollision(PhysicsComponent* component)
{
	//if (m_parent->getWorldTransform(false).hasChanged(false))
	//	updatePlanes();
	switch (component->getType())
	{
	case PhysicsComponentType::SPHERE_COMPONENT:
		return sphereInFrustum((SphereComponent*)component);
	default:
		return false;
	}
}

void FrustumComponent::updatePlanes()
{
	Transform t = m_parent->getWorldTransform(false);
	glm::vec3 centerFar = t.position() + t.forward() * m_far;
	glm::vec3 ftl = centerFar + (t.up() * m_heightFar / 2.0f) - (t.right() * m_widthFar / 2.0f); // far top left
	glm::vec3 ftr = centerFar + (t.up() * m_heightFar / 2.0f) + (t.right() * m_widthFar / 2.0f); // far top right
	glm::vec3 fbl = centerFar - (t.up() * m_heightFar / 2.0f) - (t.right() * m_widthFar / 2.0f); // far bottom left
	glm::vec3 fbr = centerFar - (t.up() * m_heightFar / 2.0f) + (t.right() * m_widthFar / 2.0f); // far bottom right

	glm::vec3 centerNear = t.position() + t.forward() * m_near;
	glm::vec3 ntl = centerNear + (t.up() * m_heightNear / 2.0f) - (t.right() * m_widthNear / 2.0f); // near top left
	glm::vec3 ntr = centerNear + (t.up() * m_heightNear / 2.0f) + (t.right() * m_widthNear / 2.0f); // near top right
	glm::vec3 nbl = centerNear - (t.up() * m_heightNear / 2.0f) - (t.right() * m_widthNear / 2.0f); // near bottom left
	glm::vec3 nbr = centerNear - (t.up() * m_heightNear / 2.0f) + (t.right() * m_widthNear / 2.0f); // near bottom right

	m_planes[TOP_PLANE].setPoints(ntr, ntl, ftl);
	m_planes[BOTTOM_PLANE].setPoints(nbl, nbr, fbr);
	m_planes[LEFT_PLANE].setPoints(ntl, nbl, fbl);
	m_planes[RIGHT_PLANE].setPoints(nbr, ntr, fbr);
	m_planes[NEAR_PLANE].setPoints(ntl, ntr, nbr);
	m_planes[FAR_PLANE].setPoints(ftr, ftl, fbl);
}

void FrustumComponent::start()
{

}

bool FrustumComponent::sphereInFrustum(SphereComponent* sphere)
{
	float distance;
	for (int i = 0; i < NUM_PLANES; i++)
	{
		distance = m_planes[i].getDistance(sphere->getOrigin());
		if (distance < -sphere->getRange())
			return false;
	}

	return true;
}
