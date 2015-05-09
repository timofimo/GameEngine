#pragma once

/*local includes*/
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "Display.h"
#include "Components/Script/Camera.h"

/*utility includes*/
#include <iostream>

class Shader
{
private:
	enum ShaderTypes
	{
		VERTEX_SHADER,
		FRAGMENT_SHADER,
		NUM_SHADERS
	};
	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	GLuint VPMatrixID, modelMatrixID;
	static Camera* m_activeCamera;

public:
	Shader(const std::string vertexShader, const std::string fragmentShader);
	~Shader();

	static void setActiveCamera(Camera* cam);
	void updateCamera();

	void bind();

	void updateModelMatrix(glm::mat4& modelMatrix);

private:
	std::string loadShader(const std::string file);
	void checkShader();
};

