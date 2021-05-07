#include <Windows.h>
#include <d3d11.h>
#include <iostream>

#include "IRenderer.h"
#include "OpenGLRenderer.h"
#include "DirectXRenderer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	printf("fuck this shit");

	////Create object MyRenderer1
	//IRenderer *MyRenderer1;
	//MyRenderer1 = new OpenGLRenderer();
	//if (!MyRenderer1->Run())
	//	return 1;

	IRenderer *MyRenderer2;
	MyRenderer2 = new DirectXRenderer(hInstance);
	
		if (!MyRenderer2->Init())
			return 1; 

	//Otherwise, call our application loop
	return (MyRenderer2->Run());

	//delete MyRenderer1;
	//delete MyRenderer2;
	return 0;
}


