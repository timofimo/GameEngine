#pragma once

/*local includes*/
#include "Shader.h"

class AmbientShader : public Shader
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
		NUM_UNIFORMS
	};
	GLuint m_uniforms[NUM_UNIFORMS];
public:
	AmbientShader();
	~AmbientShader();

	GLuint getModelMatrixID();

	void setAmbientColor(glm::vec3 color);
};

