#pragma once

/*local includes*/
#include "Shader.h"

class InstanceDirectionalShadowShader : public Shader
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
		MVP,
		NUM_UNIFORMS
	};
	GLuint m_uniforms[NUM_UNIFORMS];
	GLuint modelMatrixID;
public:
	InstanceDirectionalShadowShader();
	~InstanceDirectionalShadowShader();

	GLuint getModelMatrixID();

	void setDepthVP(glm::mat4& mvp);
};

