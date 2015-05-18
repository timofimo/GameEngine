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

	glm::vec3& position();
	glm::vec3& scale();
	glm::vec3& rotation();
	glm::quat& rotationQ();
	glm::mat4 modelMatrix();

	void setRotation(const glm::vec3 rot);
	void setRotation(const glm::quat rot);

	void translate(const glm::vec3 amount);
	void resize(const glm::vec3 amount);
	void rotate(const glm::vec3 amount);
	void rotate(const glm::quat amount);

	glm::vec3 forward();
	glm::vec3 right();

	bool hasChanged(bool reset);
	void setHasChanged(bool changed);
};

