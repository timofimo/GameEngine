#pragma once

/*local includes*/
#include "OBJLoader.h"
#include "Components/Render/RenderComponent.h"
#include "Components/Physics/SphereComponent.h"
#include "Vertex.h"
#include "gl/glew.h"

/*utility includes*/
#include <vector>
#include <map>

class Mesh
{
private:
	static OBJLoader m_loader; // used to load a mesh from OBJ file
	static std::map<std::string, Mesh*> m_loadedMeshes; // contains all loaded meshes

	std::string m_name; // name of the mesh

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	GLuint vao;
	GLuint vbo, indexBuffer, instanceBuffer;
	unsigned int m_nReferences;
	unsigned int m_nInstances;

	glm::vec3 m_center;
	float m_radius;
	glm::vec3 m_min, m_max;
	bool m_boundingBoxIsSmaller;
public:
	Mesh::Mesh(const std::string file, std::vector<Vertex> vertices, std::vector<unsigned int> indices, 
		glm::vec3 boundingSphereCenter, float radius, glm::vec3 boundingBoxMin, glm::vec3 boundingBoxMax);
	~Mesh();

	std::string getName();
	static Mesh* getMesh(std::string file);
	void release();

	void bind();
	void draw();
	void drawInstanced(std::vector<glm::mat4> instanceData, GLuint attribID);
	void unbind();

	void increaseReferences();

	glm::vec3 getBoundingSphereCenter();
	float getRadius();
	void getBoundingBox(glm::vec3& min, glm::vec3& max);
	bool boundingBoxIsSmaller();
};

