#pragma once

/*local includes*/
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "../Display.h"
#include "../Components/Script/Camera.h"

/*utility includes*/
#include <iostream>
#include <map>

class Shader
{
private:
	enum ShaderTypes
	{
		VERTEX_SHADER,
		FRAGMENT_SHADER,
		NUM_SHADERS
	};
	GLuint m_shaders[NUM_SHADERS];

	enum DataTypes
	{
		BOOLEAN,
		INTEGER,
		UNSIGNED_INTEGER,
		FLOAT,
		DOUBLE,
		VECTOR2,
		VECTOR3,
		VECTOR4,
		MATRIX3X3,
		MATRIX4X4,
		SAMPLER_2D,
	};

	class Storage
	{
	public:
		DataTypes type;
		GLuint locationID;
	};
protected:	
	std::map<std::string, Storage> m_uniforms;
	std::map<std::string, Storage> m_buffers;
	GLuint m_program;
public:
	Shader(std::string name, std::string vertexShader, std::string fragmentShader);
	virtual ~Shader();

	void setUniform(std::string uniformName, bool value);
	void setUniform(std::string uniformName, int value);
	void setUniform(std::string uniformName, unsigned int value);
	void setUniform(std::string uniformName, float value);
	void setUniform(std::string uniformName, double value);
	void setUniform(std::string uniformName, glm::vec2 value);
	void setUniform(std::string uniformName, glm::vec3 value);
	void setUniform(std::string uniformName, glm::mat3& value);
	void setUniform(std::string uniformName, glm::mat4& value);
	void setSamplerID(std::string samplerName, int id);
	unsigned int getBufferID(std::string bufferName);

	void bind();

protected:
	std::string loadShader(const std::string file);
	void checkShader(GLuint* shaders, unsigned int nShaders, std::string file);
	void loadUniforms(std::string shaderSource);
	void loadBuffers(std::string shaderSource);
	void setType(Storage* storage, std::string typeName);
};

