#include "InstancePointShader.h"


InstancePointShader::InstancePointShader()
{
	// load the shaders
	std::string vertexShaderSource = loadShader("res/Shaders/instanceVertexShader.glsl");
	std::string fragmentShaderSource = loadShader("res/Shaders/pointFragmentShader.glsl");

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
	checkShader(m_shaders, NUM_SHADERS, "Point");

	bind();
	VPMatrixID = glGetUniformLocation(m_program, "VPMatrix");
	modelMatrixID = glGetAttribLocation(m_program, "ModelMatrix");

	m_uniforms[LIGHT_COLOR] = glGetUniformLocation(m_program, "lightColor");
	m_uniforms[LIGHT_POSITION] = glGetUniformLocation(m_program, "lightPosition");
	m_uniforms[LIGHT_CONSTANT] = glGetUniformLocation(m_program, "lightConstant");
	m_uniforms[LIGHT_LINEAR] = glGetUniformLocation(m_program, "lightLinear");
	m_uniforms[LIGHT_EXPONENT] = glGetUniformLocation(m_program, "lightExponent");
	m_uniforms[EYE_POSITION] = glGetUniformLocation(m_program, "eyePosition");
	m_uniforms[SPECULAR_INTENSITY] = glGetUniformLocation(m_program, "specularIntensity");
	m_uniforms[SPECULAR_POWER] = glGetUniformLocation(m_program, "specularPower");
}


InstancePointShader::~InstancePointShader()
{
}

GLuint InstancePointShader::getModelMatrixID()
{
	return modelMatrixID;
}

void InstancePointShader::setLightColor(glm::vec3& color)
{
	glUniform3fv(m_uniforms[LIGHT_COLOR], 1, &color[0]);
}

void InstancePointShader::setLightPosition(glm::vec3 position)
{
	glUniform3fv(m_uniforms[LIGHT_POSITION], 1, &position[0]);
}

void InstancePointShader::setLightConstant(float constant)
{
	glUniform1f(m_uniforms[LIGHT_CONSTANT], constant);
}

void InstancePointShader::setLightLinear(float linear)
{
	glUniform1f(m_uniforms[LIGHT_LINEAR], linear);
}

void InstancePointShader::setLightExponent(float exponent)
{
	glUniform1f(m_uniforms[LIGHT_EXPONENT], exponent);
}

void InstancePointShader::setEyePosition(glm::vec3 position)
{
	glUniform3fv(m_uniforms[EYE_POSITION], 1, &position[0]);
}

void InstancePointShader::setSpecularIntensity(float intensity)
{
	glUniform1f(m_uniforms[SPECULAR_INTENSITY], intensity);
}

void InstancePointShader::setSpecularPower(float power)
{
	glUniform1f(m_uniforms[SPECULAR_POWER], power);
}
