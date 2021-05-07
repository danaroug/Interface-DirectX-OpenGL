#pragma once

#include "IRenderer.h"
#include <windows.h> //Include the windows header file, This contains all you will need to create a basic window
#include <string>	//needed for std::string
#include <d3d9.h>
#include <d3dx9.h>

//SAFE_DELETE MACRO used to safely delete pointer objects from memory
#define SAFE_DELETE(x) { if(x) delete x; x = NULL; }

#define WIN32_LEAN_AND_MEAN  //strips away any nonessentials (i.e. winsockets, encryption, etc.)
//we only really care about the standard application stuff

#include <windows.h> //Include the windows header file, This contains all you will need to create a basic window
#include <string>	//needed for std::string

class DirectXRenderer : public IRenderer
{
public:
	DirectXRenderer(HINSTANCE hInstance);//Constructor
	~DirectXRenderer();//Destructor

	//Main Application loop
	int Run();
	//Methods
	bool Init();
	bool Init_Window();
	void Init_Graphics();
	void Display();
	//Message Procedure function
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); 
	
	/*void Resize();
	void Motion();
	void Mouse();*/

protected:
	HWND			m_hAppWindow;				//HANDLE to application window
	HINSTANCE	m_hAppInstance;			//HANDLE to application instance
	UINT			m_ClientWidth;			//Requested client width
	UINT			m_ClientHeight;			//Requested client height
	std::string	m_AppTitle;				//Application title (window title bar)
	DWORD		m_WindowStyle;			//Window style (e.g. WS_OVERLAPPEDWINDOW)
	bool			m_EnableFullscreen;		//True to enable fullscreen, false otherwise

	LPDIRECT3D9 d3d;							//Pointer to our Direct3D interface
	LPDIRECT3DDEVICE9 d3ddev;					//Pointer to the device class
	LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;    //Pointer to the vertex buffer
	LPDIRECT3DINDEXBUFFER9 i_buffer;			//Pointer to the index buffer
};

 