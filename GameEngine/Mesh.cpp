#include "Mesh.h"

OBJLoader Mesh::loader;
std::map<std::string, Mesh*> Mesh::m_meshes;

Mesh::Mesh(const std::string file, std::vector<Vertex> vertices, std::vector<unsigned int> indices) : m_file(file)
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

	glBindVertexArray(0);
}


Mesh::~Mesh()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	m_meshes.erase(m_file);
}

Mesh* Mesh::loadMesh(std::string file)
{
	if (m_meshes.find(file) == m_meshes.end())
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		loader.loadOBJ(file, vertices, indices);
		m_meshes[file] = new Mesh(file, vertices, indices);
	}
	m_meshes[file]->m_nPointers++;
	return m_meshes[file];
}

void Mesh::release()
{
	m_nPointers--;
	if (m_nPointers == 0)
		delete this;
}

void Mesh::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
