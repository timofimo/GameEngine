#pragma once

/*local includes*/
#include "ScriptComponent.h"

class Camera : public ScriptComponent
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
	bool m_updatePlanes;
public:
	Camera(float fov = 70.0f, float zNear = 0.1f, float zFar = 1000.0f);
	~Camera();

	void update(float deltaTime);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getVPMatrix();
	Transform& getLocalTransform();

	bool pointInFrustum(glm::vec3 point);
	bool sphereInFrustum(glm::vec3 center, float radius);
	bool boxInFrustum(glm::vec3 min, glm::vec3 max, glm::quat rotation = glm::quat());

private:
	void updatePlanes();
};

