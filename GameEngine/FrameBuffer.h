#pragma once

/*local includes*/
#include "GL/glew.h"

class FrameBuffer
{
private:
	GLuint m_frameBuffer, m_renderBuffer;
	GLuint* m_textureID;
	unsigned int m_numTextures;
	unsigned int m_width, m_height;
public:
	FrameBuffer(unsigned int width, unsigned int height, unsigned int nBuffers, bool hasDepth);
	~FrameBuffer();

	void bindTexture(unsigned int buffer, unsigned int textureUnit);
	void bindAsRenderTarget();
};

