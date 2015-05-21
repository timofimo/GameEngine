#pragma once

/*local includes*/
#include "Shader.h"

class InstancePointShader : public Shader
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
		LIGHT_COLOR,
		LIGHT_POSITION,
		LIGHT_CONSTANT,
		LIGHT_LINEAR,
		LIGHT_EXPONENT,
		EYE_POSITION,
		SPECULAR_INTENSITY,
		SPECULAR_POWER,
		NUM_UNIFORMS
	};
	GLuint m_uniforms[NUM_UNIFORMS];
	GLuint modelMatrixID;
public:
	InstancePointShader();
	~InstancePointShader();

	GLuint getModelMatrixID();

	void setLightColor(glm::vec3& color);
	void setLightPosition(glm::vec3 position);
	void setLightConstant(float constant);
	void setLightLinear(float linear);
	void setLightExponent(float exponent);
	void setEyePosition(glm::vec3 position);
	void setSpecularIntensity(float intensity);
	void setSpecularPower(float power);
};

