#include "Shader.h"

Camera* Shader::m_activeCamera;

/*utility includes*/
#include <fstream>
#include <string>

Shader::Shader(const std::string vertexShader, const std::string fragmentShader)
{
	// load the shaders
	std::string vertexShaderSource = loadShader(vertexShader);
	std::string fragmentShaderSource = loadShader(fragmentShader);

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
	checkShader();

	bind();
	VPMatrixID = glGetUniformLocation(m_program, "VPMatrix");
	modelMatrixID = glGetUniformLocation(m_program, "ModelMatrix");
}


Shader::~Shader()
{
	for (int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}

	glDeleteProgram(m_program);
}

void Shader::setActiveCamera(Camera* cam)
{
	m_activeCamera = cam;
}

void Shader::updateCamera()
{
	if (m_activeCamera)
	{		
		glUniformMatrix4fv(VPMatrixID, 1, GL_FALSE, &m_activeCamera->getVPMatrix()[0][0]);
	}
}

void Shader::bind()
{
	glUseProgram(m_program);
}

void Shader::updateModelMatrix(glm::mat4& modelMatrix)
{
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
}

std::string Shader::loadShader(const std::string file)
{
	std::string result;
	std::ifstream stream(file.c_str());
	if (stream.is_open())
	{
		std::string line = "";
		while (stream.good())
		{
			std::getline(stream, line);
			result.append(line + "\n");
		}
		stream.close();
	}
	else
		std::cerr << "ERROR: could not open shader file " << file << std::endl;		

	return result;
}

void Shader::checkShader()
{
	GLint succes = 0;
	GLchar error[1024] = { 0 };

	for (int i = 0; i < NUM_SHADERS; i++)
	{
		glGetShaderiv(m_shaders[i], GL_COMPILE_STATUS, &succes);

		if (succes == GL_FALSE)
		{
			glGetShaderInfoLog(m_shaders[i], sizeof(error), NULL, error);
			std::cerr << "SHADER ERROR: " << error << std::endl;
		}
	}

	glGetProgramiv(m_program, GL_LINK_STATUS, &succes);
	if (succes == GL_FALSE)
	{
		glGetProgramInfoLog(m_program, sizeof(error), NULL, error);
		std::cerr << "SHADER PROGRAM ERROR: " << error << std::endl;
	}
}
