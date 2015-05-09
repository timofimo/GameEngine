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
}

Transform::Transform(const glm::vec3 pos)
{
	m_pos = pos;
	m_scl = glm::vec3(1.0f, 1.0f, 1.0f);
	m_rot = glm::vec3();
	m_qrot = glm::quat(m_rot);
}

Transform::Transform(const glm::vec3 pos, const glm::vec3 scl, const glm::vec3 rot)
{
	m_pos = pos;
	m_scl = scl;
	m_rot = rot;
	correctRotation();
	m_qrot = glm::quat(m_rot);
}


Transform::~Transform()
{
}

Transform Transform::operator+(Transform other)
{
	Transform temp(*this);
	temp.translate(other.position());
	temp.resize(other.scale());
	temp.rotate(other.rotationQ());
	return temp;
}

Transform& Transform::operator+=(Transform other)
{
	translate(other.position());
	resize(other.scale());
	rotate(other.rotationQ());
	return *this;
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

glm::vec3& Transform::position()
{
	return m_pos;
}

glm::vec3& Transform::scale()
{
	return m_scl;
}

glm::vec3& Transform::rotation()
{
	return m_rot;
}

glm::quat& Transform::rotationQ()
{
	return m_qrot;
}

glm::mat4 Transform::modelMatrix()
{
	return glm::translate(m_pos) * glm::mat4(m_qrot) * glm::scale(m_scl);
	//return glm::scale(m_scl) * glm::mat4(m_qrot) * glm::translate(m_pos);
}

void Transform::setRotation(const glm::vec3 rot)
{
	m_rot = rot;
	correctRotation();
	m_qrot = glm::quat(rot);
}

void Transform::setRotation(const glm::quat rot)
{
	m_qrot = rot;
	m_rot = glm::eulerAngles(m_qrot);
}

void Transform::translate(const glm::vec3 amount)
{
	m_pos += amount;
}

void Transform::resize(const glm::vec3 amount)
{
	m_scl *= amount;
}

void Transform::rotate(const glm::vec3 amount)
{
	m_rot += amount;
	correctRotation();
	m_qrot = glm::quat(m_rot);
}

void Transform::rotate(const glm::quat amount)
{
	m_qrot += amount;
	m_rot = glm::eulerAngles(m_qrot);
}

void Transform::correctRotation()
{
	m_rot.x = fmodf(m_rot.x + glm::pi<float>(), glm::two_pi<float>()) - glm::pi<float>();
	m_rot.y = fmodf(m_rot.y + glm::pi<float>(), glm::two_pi<float>()) - glm::pi<float>();
	m_rot.z = fmodf(m_rot.z + glm::pi<float>(), glm::two_pi<float>()) - glm::pi<float>();
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
