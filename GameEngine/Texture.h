#pragma once

/*local includes*/
#include "GL/glew.h"
/*utility includes*/
#include <iostream>
#include <map>

class Texture
{
private:
	static std::map<std::string, Texture*> m_loadedTextures;

	const std::string m_name;
	unsigned char* m_imageData;
	int m_width, m_height, m_nComponents;
	GLuint* m_texture;
	unsigned int m_nReferences;
public:
	Texture(std::string file);
	~Texture();

	std::string getName();
	static Texture* getTexture(std::string file);
public:
	void release();

	int getNComponents();
	void bind(unsigned int unit);
	void unbind(unsigned int unit);

private:
	void increaseReferences();
};

