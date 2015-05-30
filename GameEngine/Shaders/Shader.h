#pragma once

/*local includes*/
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "../Display.h"
#include "../Components/Script/Camera.h"

/*utility includes*/
#include <iostream>

class Shader
{
protected:
	
	GLuint m_program;
	GLuint VPMatrixID = UINT_MAX, ModelMatrixID = UINT_MAX;
public:
	Shader();
	virtual ~Shader();

	void updateVPMatrix(glm::mat4& VPMatrix);
	void UpdateModelMatrix(glm::mat4& ModelMatrix);

	void bind();

protected:
	std::string loadShader(const std::string file);
	void checkShader(GLuint* shaders, unsigned int nShaders, std::string file);
};

