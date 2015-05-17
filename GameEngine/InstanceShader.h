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
	GLuint modelMatrixID;
public:
	InstanceShader();
	~InstanceShader();

	GLuint getModelMatrixID();
};

