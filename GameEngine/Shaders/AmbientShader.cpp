#include "AmbientShader.h"


AmbientShader::AmbientShader()
{
	// load the shaders
	std::string vertexShaderSource = loadShader("res/Shaders/vertexShader.glsl");
	std::string fragmentShaderSource = loadShader("res/Shaders/ambientFragmentShader.glsl");

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
	checkShader(m_shaders, NUM_SHADERS, "Ambient");

	bind();
	VPMatrixID = glGetUniformLocation(m_program, "VPMatrix");
	ModelMatrixID = glGetUniformLocation(m_program, "ModelMatrix");

	m_uniforms[AMBIENT_COLOR] = glGetUniformLocation(m_program, "ambientColor");
}


AmbientShader::~AmbientShader()
{
}

GLuint AmbientShader::getModelMatrixID()
{
	return ModelMatrixID;
}

void AmbientShader::setAmbientColor(glm::vec3 color)
{
	glUniform3fv(m_uniforms[AMBIENT_COLOR], 1, &color[0]);
}
