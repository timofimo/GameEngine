#pragma once

/*local includes*/
#include "Shader.h"

class InstanceShader : public Shader
{
private:
	enum ShaderTypes
	{
		VERTEX_SHADER,
		FRAGMENT_SHADER,
		NUM_SHADERS
	};
	GLuint m_shaders[NUM_SHADERS];

	enum Uniforms
	{
		AMBIENT_COLOR,
		LIGHT_COLOR,
		LIGHT_DIRECTION,
		EYE_POSITION,
		SPECULAR_INTENSITY,
		SPECULAR_POWER,
		NUM_UNIFORMS
	};
	GLuint m_uniforms[NUM_UNIFORMS];
	GLuint modelMatrixID;
public:
	InstanceShader();
	~InstanceShader();

	GLuint getModelMatrixID();

	void setAmbientColor(glm::vec3 color);
	void setLightColor(glm::vec4& color);
	void setLightDirection(glm::vec3 direction);
	void setEyePosition(glm::vec3 position);
	void setSpecularIntensity(float intensity);
	void setSpecularPower(float power);
};

