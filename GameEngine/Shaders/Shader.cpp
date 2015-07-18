#include "Shader.h"

/*utility includes*/
#include <fstream>
#include <string>

Shader::Shader(std::string name, std::string vertexShader, std::string fragmentShader)
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
	checkShader(m_shaders, NUM_SHADERS, name);

	bind();

	// load all uniforms
	loadUniforms(vertexShaderSource);
	loadUniforms(fragmentShaderSource);

	// load all buffer IDs
	loadBuffers(vertexShaderSource);
	loadBuffers(fragmentShaderSource);
}


Shader::~Shader()
{
	glDeleteProgram(m_program);
}

void Shader::bind()
{
	glUseProgram(m_program);
}

void Shader::setUniform(std::string uniformName, bool value)
{
	if (m_uniforms.count(uniformName) != 0)
	{
		Storage uni = m_uniforms[uniformName];
		if (uni.type != BOOLEAN)
			return;
		glUniform1i(uni.locationID, value);
	}
}

void Shader::setUniform(std::string uniformName, int value)
{
	if (m_uniforms.count(uniformName) != 0)
	{
		Storage uni = m_uniforms[uniformName];
		if (uni.type != INTEGER)
			return;
		glUniform1i(uni.locationID, value);
	}
}

void Shader::setUniform(std::string uniformName, unsigned int value)
{
	if (m_uniforms.count(uniformName) != 0)
	{
		Storage uni = m_uniforms[uniformName];
		if (uni.type != UNSIGNED_INTEGER)
			return;
		glUniform1ui(uni.locationID, value);
	}
}

void Shader::setUniform(std::string uniformName, float value)
{
	if (m_uniforms.count(uniformName) != 0)
	{
		Storage uni = m_uniforms[uniformName];
		if (uni.type != FLOAT)
			return;
		glUniform1f(uni.locationID, value);
	}
}

void Shader::setUniform(std::string uniformName, double value)
{
	if (m_uniforms.count(uniformName) != 0)
	{
		Storage uni = m_uniforms[uniformName];
		if (uni.type != DOUBLE)
			return;
		glUniform1d(uni.locationID, value);
	}
}

void Shader::setUniform(std::string uniformName, glm::vec2 value)
{
	if (m_uniforms.count(uniformName) != 0)
	{
		Storage uni = m_uniforms[uniformName];
		if (uni.type != VECTOR2)
			return;
		glUniform2fv(uni.locationID, 1, &value[0]);
	}
}

void Shader::setUniform(std::string uniformName, glm::vec3 value)
{
	if (m_uniforms.count(uniformName) != 0)
	{
		Storage uni = m_uniforms[uniformName];
		if (uni.type != VECTOR3)
			return;
		glUniform3fv(uni.locationID, 1, &value[0]);
	}
}

void Shader::setUniform(std::string uniformName, glm::mat3& value)
{
	if (m_uniforms.count(uniformName) != 0)
	{
		Storage uni = m_uniforms[uniformName];
		if (uni.type != MATRIX3X3)
			return;
		glUniformMatrix3fv(uni.locationID, 1, GL_FALSE, &value[0][0]);
	}
}

void Shader::setUniform(std::string uniformName, glm::mat4& value)
{
	if (m_uniforms.count(uniformName) != 0)
	{
		Storage uni = m_uniforms[uniformName];
		if (uni.type != MATRIX4X4)
			return;
		glUniformMatrix4fv(uni.locationID, 1, GL_FALSE, &value[0][0]);
	}
}

void Shader::setSamplerID(std::string samplerName, int id)
{
	if (m_uniforms.count(samplerName) != 0)
	{
		Storage uni = m_uniforms[samplerName];
		if (uni.type != SAMPLER_2D)
			return;
		glUniform1i(uni.locationID, id);
	}
}

unsigned int Shader::getBufferID(std::string bufferName)
{
	if (m_buffers.count(bufferName) != 0)
	{
		Storage buf = m_buffers[bufferName];
		return buf.locationID;
	}

	return 0;
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

void Shader::loadUniforms(std::string shaderSource)
{
	size_t uniformPos = 0;
	while (true)
	{
		uniformPos = shaderSource.find("uniform ", uniformPos + 1);
		if (uniformPos == UINT_MAX)
			break;
		size_t uniformTypePos = shaderSource.find(" ", uniformPos) + 1;
		size_t uniformTypeLength = shaderSource.find(" ", uniformTypePos) - uniformTypePos;
		size_t uniformNamePos = shaderSource.find(" ", uniformTypePos) + 1;
		size_t uniformNameLength = shaderSource.find(";", uniformNamePos) - uniformNamePos;

		std::string uniformType = shaderSource.substr(uniformTypePos, uniformTypeLength);
		std::string uniformName = shaderSource.substr(uniformNamePos, uniformNameLength);

		Storage uni;
		setType(&uni, uniformType);

		uni.locationID = glGetUniformLocation(m_program, uniformName.c_str());
		if (uni.locationID != UINT_MAX)
			m_uniforms[uniformName] = uni;
	}
}

void Shader::loadBuffers(std::string shaderSource)
{
	size_t bufferPos = 0;
	while (true)
	{
		bufferPos = shaderSource.find("in ", bufferPos + 1);
		if (bufferPos == UINT_MAX)
			break;
		size_t bufferTypePos = shaderSource.find(" ", bufferPos) + 1;
		size_t bufferTypeLength = shaderSource.find(" ", bufferTypePos) - bufferTypePos;
		size_t bufferNamePos = shaderSource.find(" ", bufferTypePos) + 1;
		size_t bufferNameLength = shaderSource.find(";", bufferNamePos) - bufferNamePos;

		std::string bufferType = shaderSource.substr(bufferTypePos, bufferTypeLength);
		std::string bufferName = shaderSource.substr(bufferNamePos, bufferNameLength);

		Storage buf;
		setType(&buf, bufferType);

		buf.locationID = glGetAttribLocation(m_program, bufferName.c_str());
		if (buf.locationID != UINT_MAX)
			m_buffers[bufferName] = buf;
	}

	bufferPos = 0;
	while (true)
	{
		bufferPos = shaderSource.find("out ", bufferPos + 1);
		if (bufferPos == UINT_MAX)
			break;
		size_t bufferTypePos = shaderSource.find(" ", bufferPos) + 1;
		size_t bufferTypeLength = shaderSource.find(" ", bufferTypePos) - bufferTypePos;
		size_t bufferNamePos = shaderSource.find(" ", bufferTypePos) + 1;
		size_t bufferNameLength = shaderSource.find(";", bufferNamePos) - bufferNamePos;

		std::string bufferType = shaderSource.substr(bufferTypePos, bufferTypeLength);
		std::string bufferName = shaderSource.substr(bufferNamePos, bufferNameLength);

		Storage buf;
		setType(&buf, bufferType);

		buf.locationID = glGetAttribLocation(m_program, bufferName.c_str());
		if (buf.locationID != UINT_MAX)
			m_buffers[bufferName] = buf;
	}
}

void Shader::setType(Storage* storage, std::string typeName)
{
	if (typeName == "bool")
		storage->type = BOOLEAN;
	else if (typeName == "int")
		storage->type = INTEGER;
	else if (typeName == "uint")
		storage->type = UNSIGNED_INTEGER;
	else if (typeName == "float")
		storage->type = FLOAT;
	else if (typeName == "double")
		storage->type = DOUBLE;
	else if (typeName == "vec2")
		storage->type = VECTOR2;
	else if (typeName == "vec3")
		storage->type = VECTOR3;
	else if (typeName == "vec4")
		storage->type = VECTOR4;
	else if (typeName == "mat3")
		storage->type = MATRIX3X3;
	else if (typeName == "mat4")
		storage->type = MATRIX4X4;
	else if (typeName == "sampler2D")
		storage->type = SAMPLER_2D;
}
