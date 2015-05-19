#include "Mesh.h"

OBJLoader Mesh::m_loader;
std::map<std::string, Mesh*> Mesh::m_loadedMeshes;

Mesh::Mesh(const std::string file, std::vector<Vertex> vertices, std::vector<unsigned int> indices) : m_name(file)
{
	m_vertices = vertices;
	m_indices = indices;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)20);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &instanceBuffer);
	
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);

	m_loadedMeshes[file] = this;
}


Mesh::~Mesh()
{
	std::map<std::string, Mesh*>::iterator it = m_loadedMeshes.find(m_name);
	if (it == m_loadedMeshes.end())
		std::cout << "MESH: " << m_name.c_str() << " This mesh is unknown" << std::endl;
	else
		m_loadedMeshes.erase(it);

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

std::string Mesh::getName()
{
	return m_name;
}

Mesh* Mesh::getMesh(std::string file)
{
	std::map<std::string, Mesh*>::iterator it = m_loadedMeshes.find(file);
	Mesh* result = nullptr;

	if (it == m_loadedMeshes.end())
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		m_loader.loadOBJ(file, vertices, indices);
		result = new Mesh(file, vertices, indices);
	}
	else
		result = it->second;

	result->increaseReferences();
	return result;
}

void Mesh::release()
{
	m_nReferences--;
	if (m_nReferences <= 0)
	{
		std::cout << "MESH: " << m_name.c_str() << " was deleted" << std::endl;
		delete this;
	}
}

void Mesh::bind()
{
	glBindVertexArray(vao);
}

void Mesh::draw()
{
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::drawInstanced(std::vector<glm::mat4> instanceData, GLuint attribID)
{
	if (instanceData.size() > 0)
	{
		//update instance buffer
		glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
		glBufferData(GL_ARRAY_BUFFER, instanceData.size() * sizeof(glm::mat4), &instanceData[0][0][0], GL_DYNAMIC_DRAW);
		for (int i = 0; i < 4; i++)
		{
			unsigned int pointer = i * sizeof(glm::vec4);
			glEnableVertexAttribArray(attribID + i);
			glVertexAttribPointer(attribID + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)pointer);
			glVertexAttribDivisor(attribID + i, 1);//WorldMatrix
		}

		glVertexAttribDivisor(0, 0);//vertex position
		glVertexAttribDivisor(1, 0);//texCoord
		glVertexAttribDivisor(2, 0);//normal

		m_nInstances = instanceData.size();
	}

	glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0, m_nInstances);

	/*GLuint error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "Error: " << error << std::endl;
	}*/
}

void Mesh::unbind()
{
	glBindVertexArray(0);
}

void Mesh::increaseReferences()
{
	m_nReferences++;
}
