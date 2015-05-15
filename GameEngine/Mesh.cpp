#include "Mesh.h"

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
}

unsigned int Mesh::addComponent(RenderComponent* component)
{
	if (std::find(m_components.begin(), m_components.end(), component) == m_components.end())
	{
		m_components.push_back(component);
	}
	else
	{
		std::cout << m_file.c_str() << " already had a component " << component->getName().c_str() << std::endl;
	}

	return m_components.size();
}

unsigned int Mesh::releaseComponent(RenderComponent* component)
{
	std::vector<RenderComponent*>::iterator it = std::find(m_components.begin(), m_components.end(), component);
	if (it == m_components.end())
	{
		std::cout << "You tried to release " << component->getName().c_str() << " from " << m_file.c_str() << ". But it doesn't have a pointer to this component." << std::endl;
		return m_components.size();
	}

	m_components.erase(it);

	return m_components.size();
}

void Mesh::bind()
{
	glBindVertexArray(vao);
}

void Mesh::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
