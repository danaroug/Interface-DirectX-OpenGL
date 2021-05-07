#include "DirectXRenderer.h"
#include "IRenderer.h"

#define WIN32_LEAN_AND_MEAN
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct CUSTOMVERTEX { FLOAT X, Y, Z; DWORD COLOR; };

namespace
{
	//Create global pointer to a DirectXRenderer object
	DirectXRenderer* g_ptr = NULL;
}

//This is used to forward messages to our used defined procedure function
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Forward messages
	if (g_ptr)
		return (g_ptr->MsgProc(hwnd, msg, wParam, lParam));
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

//This is used to forward messages to our used defined procedure function
LRESULT DirectXRenderer::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Switch statement on the message passed to us
	switch (msg)
	{
		//CASE: WM_DESTROY, our application is told to destroy itself
	case WM_DESTROY:
		PostQuitMessage(0); //Tell the application to quit
		return 0;

		//CASE: WM_KEYDOWN, user pressed a key on keyboard
	case WM_KEYDOWN:
		//Switch on the WPARAM, (which stores the keycode)
		switch (wParam)
		{
			//CASE: VK_ESCAPE, user pressed the escape key
		case VK_ESCAPE:
			PostQuitMessage(0); //Tell application to quit
			return 0;
		}
		return 0;
	}
	//Always return the default window procedure if we don't catch anything
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

DirectXRenderer::DirectXRenderer(HINSTANCE hInstance)
{
	//Initialize members
	m_hAppInstance = hInstance;
	m_hAppWindow = NULL;
	m_AppTitle = "DirectX Cube!";
	m_ClientWidth = 300;
	m_ClientHeight = 300;
	
	m_WindowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX; //Standard non-resizeable window
	g_ptr = this; //Set our global pointer
}

DirectXRenderer::~DirectXRenderer()
{
	//Release objects from memory
	SAFE_DELETE(g_ptr, d3d, d3ddev, v_buffer, i_buffer);
}

int DirectXRenderer::Run()
{
	//Main message loop (MAIN APPLICATION LOOP)
	//For every application that remains persistent on a system there needs to be a loop of some kind
	//As I explained further down, windows applications use a message based system. While your application
	//is still receiving message that don't equal WM_QUIT it will be running on the system.
	//Here is how your create such a loop:

	//Create MSG struct
	MSG msg = { 0 }; //sets all members to null. (empty set)
	while (WM_QUIT != msg.message) //While our message doesn't equal WM_QUIT
	{
		//Peek at a message on the stack,
		//store it in msg,
		//and remove it from the stack
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg); //Translate message
			DispatchMessage(&msg); //Dispatch message
		}
		//Here is where things such as rendering, updating, etc. would go
		else
		{
			Display();
		}
	}
	
	//Now when the application finally finishes, we need to return
	//the error code given from our application
	return static_cast<int>(msg.wParam); //The error code is stored in the wParam member of our msg struct
}

bool DirectXRenderer::Init()
{
	//Initialize main window
	if (!Init_Window())
		return false;

	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = m_hAppWindow;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = m_ClientWidth;
	d3dpp.BackBufferHeight = m_ClientHeight;
	d3dpp.EnableAutoDepthStencil = TRUE;    //Automatically run the z-buffer
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;    //16-bit pixel format for the z-buffer

	//Create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_hAppWindow,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	Init_Graphics();

	d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);			//Turn off the 3D lighting
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);   //Both sides of the triangles
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);			//Turn on the z-buffer
	
	return true;
}

void DirectXRenderer::Init_Graphics()
{
	//Create the vertices using the CUSTOMVERTEX struct
	CUSTOMVERTEX vertices[] =
	{
		{ -3.0f, 3.0f, -3.0f, D3DCOLOR_XRGB(102, 0, 102), },	// vertex 0
	{ 3.0f, 3.0f, -3.0f, D3DCOLOR_XRGB(255, 255, 255), },	// vertex 1
	{ -3.0f, -3.0f, -3.0f, D3DCOLOR_XRGB(102, 0, 102), },	// 2
	{ 3.0f, -3.0f, -3.0f, D3DCOLOR_XRGB(255, 255, 255), },	// 3
	{ -3.0f, 3.0f, 3.0f, D3DCOLOR_XRGB(0, 0, 255), },		// 4
	{ 3.0f, 3.0f, 3.0f, D3DCOLOR_XRGB(102, 255, 255), },	// 5
	{ -3.0f, -3.0f, 3.0f, D3DCOLOR_XRGB(0, 255, 0), },		// 6
	{ 3.0f, -3.0f, 3.0f, D3DCOLOR_XRGB(0, 255, 255), },		// 7
	};

	//Create the indices using an int array
	short indices[] =
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 0, 6,    // side 2
		6, 0, 2,
		7, 5, 6,    // side 3
		6, 5, 4,
		3, 1, 7,    // side 4
		7, 1, 5,
		4, 5, 0,    // side 5
		0, 5, 1,
		3, 7, 2,    // side 6
		2, 7, 6,
	};

	//Create a vertex buffer interface called v_buffer
	d3ddev->CreateVertexBuffer(8 * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);

	VOID* pVoid;    //Void pointer

	//Create an index buffer interface called i_buffer
	d3ddev->CreateIndexBuffer(36 * sizeof(short),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&i_buffer,
		NULL);

	//Lock i_buffer and load the indices into it
	i_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indices, sizeof(indices));
	i_buffer->Unlock();

	//Lock v_buffer and load the vertices into it
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer->Unlock();

	//Select the vertex and index buffers to use
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	d3ddev->SetIndices(i_buffer);
}
bool DirectXRenderer::Init_Window()
{
	//First step:
	//Create a window class structure to define our window
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX)); //ZERO it out
	wcex.cbClsExtra = 0; //no extra bytes
	wcex.cbWndExtra = 0; //no extra bytes
	wcex.cbSize = sizeof(WNDCLASSEX); //set size in bytes
	wcex.style = CS_HREDRAW | CS_VREDRAW; //Basically states that window should be redraw both HORIZ. and VERT.
	wcex.hInstance = m_hAppInstance; //Set handle to application instance;
	wcex.lpfnWndProc = MainWndProc; //Set message procedure to our globally defined one
	//wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); //Set window icon (standard application icon)
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //Set window arrow (standard windows arrow)
	//wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //Set clear background
	wcex.lpszClassName = "WIN32WINDOWCLASS"; //Name it w.e you like. 
	//wcex.lpszMenuName = NULL; //We are not using a menu at this time.
	//wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION); //Set small window icon (standard application icon)

	//Now we must register the window class
	//Here is an example of some simple error checking
	//This can be quite useful for larger projects to debug errors
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Failed to register window class", NULL, NULL);
		return false;
	}

	//Second step:
	//Cache the correct window dimensions
	RECT r = { 0, 0, m_ClientWidth, m_ClientHeight };
	AdjustWindowRect(&r, m_WindowStyle, false); //Use our window style
	int width = r.right - r.left;  //correct width based on requested client size
	int height = r.bottom - r.top;  //correct height based on requested client size
	int x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2; //Centers window on desktop
	int y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2; //Centers window on desktop

	//Third step:
	//Create our window
	//lpClassName: MUST BE SAME AS ABOVE FROM WINDOW CLASS
	//lpWindowTitle: SHOULD BE m_AppTitle.c_str()
	m_hAppWindow = CreateWindow("WIN32WINDOWCLASS", m_AppTitle.c_str(), m_WindowStyle, x, y,
		width, height, NULL, NULL, m_hAppInstance, NULL);
	//Check window creation
	if (!m_hAppWindow)
	{
		MessageBox(NULL, "Failed to create window", NULL, NULL);
		return false;
	}
	
	//Fourth step:
	//Show window
	//SW_SHOW: Stand window display code, take the place of nCmdShow from entry point.
	ShowWindow(m_hAppWindow, SW_SHOW);
	
	//If all succeeded return true
	return true;
}
void DirectXRenderer::Display()
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();

	//Select which vertex format we are using
	d3ddev->SetFVF(CUSTOMFVF);

	//Set the view transform
	D3DXMATRIX matView;							//The view transform matrix
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(0.0f, 0.0f, 30.0f),		//Camera position
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),			//Look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));		//Up direction
	d3ddev->SetTransform(D3DTS_VIEW, &matView);	//Set the view transform to matView

												//Set the projection transform
	D3DXMATRIX matProjection;						//The projection transform matrix
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(40),		//The horizontal field of view
		(FLOAT)m_ClientWidth / (FLOAT)m_ClientHeight, //Aspect ratio
		1.0f,										//Near view-plane
		100.0f);									//Far view-plane
	d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);   //Set the projection
	D3DXMATRIX matTranslate;    //Matrix to store the translation information

								//Build a matrix to move the model 12 units along the x-axis and 4 units along the y-axis
								//Store it to matTranslate
	D3DXMatrixTranslation(&matTranslate, 0.0f, 0.0f, -1.0f);

	//Tell Direct3D about our matrix
	d3ddev->SetTransform(D3DTS_WORLD, &matTranslate);

	//Matrix to store the rotation information
	D3DXMATRIX matRotateY;
	D3DXMATRIX matRotateX;
	D3DXMATRIX matRotateZ;

	//Build a matrix to rotate the model
	D3DXMatrixRotationX(&matRotateX, D3DXToRadian(30.0f));
	D3DXMatrixRotationY(&matRotateY, D3DXToRadian(50.0f));
	D3DXMatrixRotationZ(&matRotateZ, D3DXToRadian(30.0f));

	//Tell Direct3D about our matrix
	d3ddev->SetTransform(D3DTS_WORLD, &(matRotateX * matRotateY * matRotateZ  * matTranslate));
	//Draw the cube
	d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}

//void DirectXRenderer::Resize()
//{
//
//}
//void DirectXRenderer::Motion()
//{
//
//}
//void DirectXRenderer::Mouse()
//{
//
//}





