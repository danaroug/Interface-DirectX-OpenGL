#pragma once

#include <iostream>

#include "IRenderer.h"

class OpenGLRenderer : public IRenderer

{
public:
	
	OpenGLRenderer();
	~OpenGLRenderer();

	int Run();
	bool Init();
	void Init_Graphics();
	void Display();

protected:
	int argc;
	char** argv;
	int width, height;
};


