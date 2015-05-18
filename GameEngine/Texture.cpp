#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION

std::map<std::string, Texture*> Texture::m_loadedTextures;

/*local includes*/
#include "stb-master/stb_image.h"

/*utility includes*/
#include <windows.h>

Texture::Texture(std::string file) : m_name(file)
{
	m_imageData = stbi_load(file.c_str(), &m_width, &m_height, &m_nComponents, 0);
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

	int glComponentType = GL_R;
	switch (m_nComponents)
	{
	case 4:
		glComponentType = GL_RGBA;
		break;
	case 3:
		glComponentType = GL_RGB;
		break;
	case 2:
		glComponentType = GL_RG;
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, glComponentType, m_width, m_height, 0, glComponentType, GL_UNSIGNED_BYTE, m_imageData);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(m_imageData);

	m_loadedTextures[file] = this;
	m_nReferences = 0;
}


Texture::~Texture()
{
	std::map<std::string, Texture*>::iterator it = m_loadedTextures.find(m_name);
	if (it == m_loadedTextures.end())
		std::cout << "ERROR TEXTURE: " << m_name.c_str() << " This texture is unknown" << std::endl;
	else
		m_loadedTextures.erase(it);

	glDeleteTextures(1, m_texture);
	if (m_texture) delete m_texture; m_texture = nullptr;
}

std::string Texture::getName()
{
	return m_name;
}

Texture* Texture::getTexture(std::string file)
{
	std::map<std::string, Texture*>::iterator it = m_loadedTextures.find(file);
	Texture* result = nullptr;

	if (it == m_loadedTextures.end())
	{
		result = new Texture(file);
	}
	else
		result = it->second;

	result->increaseReferences();
	return result;
}

void Texture::release()
{
	m_nReferences--;
	if (m_nReferences <= 0)
	{
		std::cout << "TEXTURE: " << m_name.c_str() << " was deleted" << std::endl;
		delete this;
	}
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

void Texture::unbind(unsigned int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::increaseReferences()
{
	m_nReferences++;
}
