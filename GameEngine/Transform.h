#pragma once

/*local includes*/
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

class Transform
{
private:
	glm::vec3 m_pos, m_scl, m_rot;
	glm::quat m_qrot;
	bool m_hasChanged;
	glm::mat4 m_worldMatrix;
public:
	Transform();
	Transform(const glm::vec3 pos);
	Transform(const glm::vec3 pos, const glm::vec3 scl, const glm::vec3 rot);
	~Transform();

	void printPosition();
	void printScale();
	void printRotation();
	void printRotationQ();

	glm::vec3 position();
	glm::vec3 scale();
	float scalef();
	glm::vec3 rotation();
	glm::quat rotationQ();
	glm::mat4 modelMatrix();

	Transform* setPosition(const glm::vec3 pos);
	Transform* setScale(const glm::vec3 scl);
	Transform* setRotation(const glm::vec3 rot);
	Transform* setRotation(const glm::quat rot);

	Transform* translate(const glm::vec3 amount);
	Transform* resize(const glm::vec3 amount);
	Transform* rotate(const glm::vec3 amount);
	Transform* rotate(const glm::quat amount);

	glm::vec3 forward();
	glm::vec3 right();
	glm::vec3 up();

	bool hasChanged(bool reset);
	void setHasChanged(bool changed);

	glm::vec3 transformPoint(glm::vec3 point);
};

