#pragma once

/*local includes*/
#include "PhysicsComponent.h"
#include "SphereComponent.h"

class FrustumComponent : public PhysicsComponent
{
private:
	float m_near, m_far;
	float m_heightFar, m_heightNear, m_widthFar, m_widthNear;
	float m_fov, m_ratio;

	enum FrustumPlanes
	{
		TOP_PLANE,
		BOTTOM_PLANE,
		LEFT_PLANE,
		RIGHT_PLANE,
		NEAR_PLANE,
		FAR_PLANE,
		NUM_PLANES
	};
	struct Plane
	{
	private:
		glm::vec3 points[3];
		glm::vec3 n;
	public:
		void setPoints(glm::vec3 a, glm::vec3 b, glm::vec3 c)
		{
			points[0] = a;
			points[1] = b;
			points[2] = c;

			glm::vec3 v = b - a;
			glm::vec3 u = c - a;

			n = glm::normalize(glm::cross(u, v));
		}
		float getDistance(glm::vec3 point)
		{
			return glm::dot(n, point - points[0]);
		}
	}; Plane m_planes[NUM_PLANES];
public:
	FrustumComponent(float left, float right, float bottom, float top, float zNear, float zFar);
	FrustumComponent(float fov, float zNear, float zFar);
	~FrustumComponent();

	bool checkCollision(PhysicsComponent* component);
	void updatePlanes();
	void start();
private:
	bool sphereInFrustum(SphereComponent* sphere);
};

