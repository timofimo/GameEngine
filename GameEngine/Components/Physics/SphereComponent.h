#pragma once

/*local includes*/
#include "PhysicsComponent.h"

class SphereComponent : public PhysicsComponent
{
private:
	glm::vec3 m_origin;
	float m_range;
public:
	SphereComponent(glm::vec3 origin, float range);
	~SphereComponent();

	void setOrigin(glm::vec3 origin);
	glm::vec3 getOrigin(glm::vec3 offset = glm::vec3());
	void setRange(float range);
	float getRange();
	void setSphere(glm::vec3 origin, float range);

	bool checkCollision(PhysicsComponent* component);
private:
	void start(){}
};

