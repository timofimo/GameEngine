#include "InstanceDirectionalShadowShader.h"


InstanceDirectionalShadowShader::InstanceDirectionalShadowShader()
{
	// load the shaders
	std::string vertexShaderSource = loadShader("res/Shaders/instanceVertexShadowShader.glsl");
	std::string fragmentShaderSource = loadShader("res/Shaders/fragmentShadowShader.glsl");

	// create and compile the shaders
	const GLchar* p[1];
	GLint lengths[1];

	p[0] = vertexShaderSource.c_str();
	lengths[0] = vertexShaderSource.length();
	m_shaders[VERTEX_SHADER] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_shaders[VERTEX_SHADER], 1, p, lengths);
	glCompileShader(m_shaders[VERTEX_SHADER]);

	p[0] = fragmentShaderSource.c_str();
	lengths[0] = fragmentShaderSource.length();
	m_shaders[FRAGMENT_SHADER] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_shaders[FRAGMENT_SHADER], 1, p, lengths);
	glCompileShader(m_shaders[FRAGMENT_SHADER]);

	// create, link and validate the program
	m_program = glCreateProgram();
	glAttachShader(m_program, m_shaders[VERTEX_SHADER]);
	glAttachShader(m_program, m_shaders[FRAGMENT_SHADER]);
	glLinkProgram(m_program);
	glValidateProgram(m_program);

	// check the shader for errors
	checkShader(m_shaders, NUM_SHADERS, "DirectionalShadow");

	bind();
	VPMatrixID = glGetUniformLocation(m_program, "VPMatrix");
	modelMatrixID = glGetAttribLocation(m_program, "ModelMatrix");
}


InstanceDirectionalShadowShader::~InstanceDirectionalShadowShader()
{
}

GLuint InstanceDirectionalShadowShader::getModelMatrixID()
{
	return modelMatrixID;
}

void InstanceDirectionalShadowShader::setDepthVP(glm::mat4& lightVPMatrix)
{
	glUniformMatrix4fv(VPMatrixID, 1, GL_FALSE, &lightVPMatrix[0][0]);
}
