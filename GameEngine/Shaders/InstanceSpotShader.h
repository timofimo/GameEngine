#pragma once

/*local includes*/
#include "Shader.h"

class InstanceSpotShader : public Shader
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
		LIGHT_DIRECTION,
		LIGHT_POSITION,
		LIGHT_CUTOFF,
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
	InstanceSpotShader();
	~InstanceSpotShader();

	GLuint getModelMatrixID();

	void setLightColor(glm::vec3& color);
	void setLightDirection(glm::vec3 direction);
	void setLightPosition(glm::vec3 position);
	void setLightCutOff(float cutOff);
	void setLightConstant(float constant);
	void setLightLinear(float linear);
	void setLightExponent(float exponent);
	void setEyePosition(glm::vec3 position);
	void setSpecularIntensity(float intensity);
	void setSpecularPower(float power);
};

