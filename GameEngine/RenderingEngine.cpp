#include "RenderingEngine.h"


RenderingEngine::RenderingEngine()
{
}


RenderingEngine::~RenderingEngine()
{
}

void RenderingEngine::render(GameObject* rootObject)
{
	if (!activeShader || !activeCamera)
	{
		std::cout << "There is no active shader or camera" << std::endl;
		return;
	}

	// for each mesh
		// for each texture
			// update model matrix
			// draw
}
