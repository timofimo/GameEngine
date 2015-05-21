#include "Shader.h"

/*utility includes*/
#include <fstream>
#include <string>

Shader::Shader()
{}


Shader::~Shader()
{
	glDeleteProgram(m_program);
}

void Shader::updateVPMatrix(glm::mat4& VPMatrix)
{
	if (VPMatrixID != UINT_MAX)
		glUniformMatrix4fv(VPMatrixID, 1, GL_FALSE, &VPMatrix[0][0]);
}

void Shader::bind()
{
	glUseProgram(m_program);
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

void Shader::checkShader(GLuint* shaders, unsigned int nShaders, std::string file)
{
	GLint succes = 0;
	GLchar error[1024] = { 0 };

	for (unsigned int i = 0; i < nShaders; i++)
	{
		glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &succes);

		if (succes == GL_FALSE)
		{
			glGetShaderInfoLog(shaders[i], sizeof(error), NULL, error);
			std::cerr << "SHADER ERROR: " << file.c_str() << error << std::endl;
		}
	}

	glGetProgramiv(m_program, GL_LINK_STATUS, &succes);
	if (succes == GL_FALSE)
	{
		glGetProgramInfoLog(m_program, sizeof(error), NULL, error);
		std::cerr << "SHADER PROGRAM ERROR: " << file.c_str() << error << std::endl;
	}
}
