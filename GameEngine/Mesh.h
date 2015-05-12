#pragma once

/*local includes*/
#include "Components/Render/RenderComponent.h"
#include "Vertex.h"
#include "gl/glew.h"

/*utility includes*/
#include <vector>

class Mesh
{
private:
	const std::string m_file;
	std::vector<RenderComponent*> m_components;

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	GLuint vao;
	GLuint vbo, indexBuffer;
public:
	Mesh::Mesh(const std::string file, std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Mesh();

	unsigned int addComponent(RenderComponent* component);
	unsigned int releaseComponent(RenderComponent* component);

	void draw();
};

