#pragma once

/*local includes*/
#include "OBJLoader.h"
#include "gl/glew.h"

/*utility includes*/
#include <map>

class Mesh
{
private:
	static OBJLoader loader;
	static std::map<std::string, Mesh*> m_meshes;

	const std::string m_file;
	unsigned int m_nPointers;

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	GLuint vao;
	GLuint vbo, indexBuffer;
public:
	Mesh::Mesh(const std::string file, std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Mesh();

	static Mesh* loadMesh(std::string file);
	void release();

	void draw();
};

