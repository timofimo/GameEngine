#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION

/*local includes*/
#include "stb-master/stb_image.h"

/*utility includes*/
#include <windows.h>

Texture::Texture(const std::string file, int forceComponents/* = 0*/)
{
	m_imageData = stbi_load(file.c_str(), &m_width, &m_height, &m_nComponents, forceComponents);
	if (!m_imageData)
	{
		std::string error = "Failed to load " + file;
		MessageBox(NULL, error.c_str(), "Error loading texture", MB_OK);
		exit(EXIT_FAILURE);
	}

	m_texture = new GLuint;
	glGenTextures(1, m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture[0]);	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.5f);

	int glComponentType = 0;
	if (m_nComponents == 4)
		glComponentType = GL_RGBA;
	else if (m_nComponents == 3)
		glComponentType = GL_RGB;
	else if (m_nComponents == 1)
		glComponentType = GL_R;
	else if (m_nComponents == 2)
		glComponentType = GL_RG;

	glTexImage2D(GL_TEXTURE_2D, 0, glComponentType, m_width, m_height, 0, glComponentType, GL_UNSIGNED_BYTE, m_imageData);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(m_imageData);
}


Texture::~Texture()
{
	glDeleteTextures(1, m_texture);
	if (m_texture) delete m_texture; m_texture = nullptr;
}

GLuint* Texture::getTexture()
{
	return m_texture;
}

int Texture::getNComponents()
{
	return m_nComponents;
}

void Texture::bind(unsigned int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_texture[0]);
}
