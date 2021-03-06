#include "Transform.h"

/*local includes*/
#include "glm/gtx/transform.hpp"

/*utility includes*/
#include <iostream>
#include <iomanip>

Transform::Transform()
{
	m_pos = glm::vec3();
	m_scl = glm::vec3(1.0f, 1.0f, 1.0f);
	m_rot = glm::vec3();
	m_qrot = glm::quat(m_rot);
	m_hasChanged = true;
}

Transform::Transform(const glm::vec3 pos)
{
	m_pos = pos;
	m_scl = glm::vec3(1.0f, 1.0f, 1.0f);
	m_rot = glm::vec3();
	m_qrot = glm::quat(m_rot);
	m_hasChanged = true;
}

Transform::Transform(const glm::vec3 pos, const glm::vec3 scl, const glm::vec3 rot)
{
	m_pos = pos;
	m_scl = scl;
	m_rot = rot;
	m_qrot = glm::quat(m_rot);
	m_hasChanged = true;
}


Transform::~Transform()
{
}

void Transform::printPosition()
{
	std::cout << std::setprecision(3) << "Position(" << m_pos.x << ", " << m_pos.y << ", " << m_pos.z << ")" << std::endl;
}

void Transform::printScale()
{
	std::cout << std::setprecision(3) << "Scale(" << m_scl.x << ", " << m_scl.y << ", " << m_scl.z << ")" << std::endl;
}

void Transform::printRotation()
{
	glm::vec3 tempRot = glm::degrees(m_rot);
	std::cout << std::setprecision(3) << "Rotation(" << tempRot.x << ", " << tempRot.y << ", " << tempRot.z << ")" << std::endl;
}

void Transform::printRotationQ()
{
	std::cout << std::setprecision(3) << "Rotation(" << m_qrot.x << ", " << m_qrot.y << ", " << m_qrot.z << ", " << m_qrot.w << ")" << std::endl;
}

glm::vec3 Transform::position()
{
	return m_pos;
}

glm::vec3 Transform::scale()
{
	return m_scl;
}

float Transform::scalef()
{
	return glm::max(glm::max(m_scl.x, m_scl.y), m_scl.z);
}

glm::vec3 Transform::rotation()
{
	return m_rot;
}

glm::quat Transform::rotationQ()
{
	return m_qrot;
}

glm::mat4 Transform::modelMatrix()
{
	if (m_hasChanged)
		return m_worldMatrix = glm::translate(m_pos) * glm::mat4(m_qrot) * glm::scale(m_scl);
	else
		return m_worldMatrix;
}

void Transform::setPosition(const glm::vec3 pos)
{
	m_hasChanged = true;
	m_pos = pos;
}

void Transform::setScale(const glm::vec3 scl)
{
	m_hasChanged = true;
	m_scl = scl;
}

void Transform::setRotation(const glm::vec3 rot)
{
	m_hasChanged = true;
	m_rot = rot;
	m_qrot = glm::quat(rot);
}

void Transform::setRotation(const glm::quat rot)
{
	m_hasChanged = true;
	m_qrot = rot;
	m_rot = glm::eulerAngles(m_qrot);
}

void Transform::translate(const glm::vec3 amount)
{
	m_hasChanged = true;
	m_pos += amount;
}

void Transform::resize(const glm::vec3 amount)
{
	m_hasChanged = true;
	m_scl *= amount;
}

void Transform::rotate(const glm::vec3 amount)
{
	m_hasChanged = true;
	m_rot += amount;
	m_qrot = glm::quat(m_rot);
}

void Transform::rotate(const glm::quat amount)
{
	m_hasChanged = true;
	m_qrot = amount * m_qrot;
	m_rot = glm::eulerAngles(m_qrot);
}

glm::vec3 Transform::forward()
{
	glm::vec3 forward;
	forward.x = cos(m_rot.x) * cos(m_rot.y);
	forward.y = sin(m_rot.x);
	forward.z = cos(m_rot.x) * sin(m_rot.y);
	return glm::normalize(forward);
}

glm::vec3 Transform::right()
{
	return glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward()));
}

glm::vec3 Transform::up()
{
	return glm::normalize(glm::cross(forward(), right()));
}

bool Transform::hasChanged(bool reset)
{
	bool result = m_hasChanged;
	if(reset)
		m_hasChanged = false;
	return result;
}

void Transform::setHasChanged(bool changed)
{
	m_hasChanged = changed;
}

glm::vec3 Transform::transformPoint(glm::vec3 point)
{
	glm::vec3 result = point;
	result *= m_scl;
	result = m_qrot * result;
	result += m_pos;
	return result;
}
