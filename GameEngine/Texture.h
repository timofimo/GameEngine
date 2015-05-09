#pragma once

/*local includes*/
#include "GL/glew.h"
/*utility includes*/
#include <iostream>

class Texture
{
private:
	unsigned char* m_imageData;
	int m_width, m_height, m_nComponents;
	GLuint* m_texture;
public:
	Texture(){}
	Texture(const std::string file, int forceComponents = 0);
	~Texture();

	GLuint* getTexture();
	int getNComponents();
	void bind(unsigned int unit);
};

