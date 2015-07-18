#include "FrameBuffer.h"

/*local includes*/
#include "glm/glm.hpp"
/*utility includes*/
#include <iostream>

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height, unsigned int nBuffers, bool hasDepth)
{
	m_frameBuffer = 0;
	m_renderBuffer = 0;
	m_textureID = new GLuint[nBuffers];
	m_numTextures = nBuffers;
	m_width = width;
	m_height = height;

	glGenTextures(nBuffers, m_textureID);
	for (unsigned int i = 0; i < nBuffers; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID[i]);

		if (hasDepth && i == nBuffers - 1)
		{
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)[0]);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL); 
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		}
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0); 
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	GLenum* drawBuffers = new GLenum[nBuffers];
	for (unsigned int i = 0; i < nBuffers; i++)
	{
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;

		if (hasDepth && i == nBuffers - 1)
		{
			drawBuffers[i] = GL_NONE;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_textureID[i], 0);
			continue;
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, drawBuffers[i], GL_TEXTURE_2D, m_textureID[i], 0);
	}

	if (!hasDepth)
	{
		glGenRenderbuffers(1, &m_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
	}

	glDrawBuffers(nBuffers, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Framebuffer creation failed!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


FrameBuffer::~FrameBuffer()
{
	if (*m_textureID) glDeleteTextures(m_numTextures, m_textureID);
	if (m_frameBuffer) glDeleteFramebuffers(1, &m_frameBuffer);
	if (m_renderBuffer) glDeleteRenderbuffers(1, &m_renderBuffer);
	if (m_textureID) delete[] m_textureID;
}

void FrameBuffer::bindTexture(unsigned int buffer, unsigned int textureUnit)
{
	if (buffer < m_numTextures)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, m_textureID[buffer]);
		glActiveTexture(GL_TEXTURE0);
	}
}

void FrameBuffer::bindAsRenderTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glViewport(0, 0, m_width, m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
