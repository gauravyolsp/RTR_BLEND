// win32 Headers
#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
// OpenGL Header File
#include<gl/GL.h>
#include<gl/GLU.h>

#include "OGL.h"

// Open GL relared Library
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"GLU32.lib")
// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global function declarations
LRESULT CALLBACK WndProck(HWND, UINT, WPARAM, LPARAM);          /// check with WNDPROK

// Global variable declarations
// Variable Related for FullScreen
BOOL gbFullScreen = FALSE;
HWND ghwnd = NULL;
DWORD dwStyle;
WINDOWPLACEMENT wpPrev;

// Variable Related with file I/O
char gszLogFileName[] = "Log.txt";
FILE* gpFile = NULL;

// Active Window related variable
BOOL gbActiveWindow = FALSE;

// Exit key press related 
BOOL gbExcapeKeyPressed = FALSE;

//Open GL related variables 
HDC ghdc = NULL;
HGLRC ghrc = NULL;

// Rotation Angles
GLfloat angleCube= 0;

// Entry Point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// function declarations
	int initilize(void);
	void display(void);
	void update(void);
	void uninitilize(void);

	// variable declarations
	WNDCLASSEX wndClass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("RTR6");
	BOOL bDone = FALSE;

	// code
	
	// Create Log file
	gpFile = fopen(gszLogFileName, "w");
	if (gpFile == NULL)
	{
		MessageBox(NULL, "LogFile Creation Failed !!!", "Error", MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Program Started Sucessfully !! \n");
	}
	// window class initilization
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.lpfnWndProc = WndProck;
	wndClass.hInstance = hInstance;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpszClassName = szAppName;
	wndClass.lpszMenuName = NULL;
	wndClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// Registration of window class 
	RegisterClassEx(&wndClass);

	// int iscreenWidth, iscreenHeight, iScreenPosX, iScreenPosY;

	/*iscreenWidth = GetSystemMetrics(SM_CXSCREEN);
	iscreenHeight = GetSystemMetrics(SM_CYSCREEN);

	iScreenPosX = (iscreenWidth - WIN_WIDTH) / 2;
	iScreenPosY = (iscreenHeight - WIN_HEIGHT) / 2;
	*/

	// Create window
	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		szAppName,

		TEXT("Suraj Deshmukh"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;
	
	// Show Window
	ShowWindow(hwnd, iCmdShow);

	// Paint background of the window
	UpdateWindow(hwnd);

	
	// Initilize
	int result = initilize();
	
	if (result != 0)
	{
		fprintf(gpFile, "Initilize function failed !!! \n");
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
	else
	{
		fprintf(gpFile, "Initileze function completed sucessfully !! \n");
	}

	// Set this window as foreground and Active window
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// Game Loop
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
               bDone = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow == TRUE)
			{
				if (gbExcapeKeyPressed == TRUE)
				{
					bDone = TRUE;
				}
				// Render
				display();

				// Update
				update();
			}	
		}
	}
	// Uninitilize
	uninitilize();

	return((int)msg.wParam);
}

// CALLBACK function
LRESULT CALLBACK WndProck(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// local function declarations
	void toggleFullScreen(void);
	void uninitilize(void);
	void resize(int, int);

	//code
	switch (iMsg)
	{
	case WM_CREATE:
		ZeroMemory((void*)&wpPrev, sizeof(WINDOWPLACEMENT));
		wpPrev.length = sizeof(WINDOWPLACEMENT);
		break;
	case WM_SETFOCUS :
		gbActiveWindow = TRUE;
		break;
	case WM_KILLFOCUS :
		gbActiveWindow = FALSE;
		break;
	case WM_ICONERASEBKGND:
		return 0;
	case WM_SIZE :
		resize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_KEYDOWN :
		switch (wParam)
		{
		case VK_ESCAPE :
			gbExcapeKeyPressed = TRUE;
			break;
		default:
			break;
		}
		break;
	case WM_CHAR:
		switch (wParam)
		{
		case 'F':
		case 'f':
			if (gbFullScreen == FALSE)
			{
				toggleFullScreen();
				gbFullScreen = TRUE;
			}
			else
			{
				toggleFullScreen();
				gbFullScreen = FALSE;
			}
			break;
		default:
			break;
		}
		break;
	case WM_CLOSE :
		uninitilize();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void toggleFullScreen(void)
{
	//variable declarations
	MONITORINFO mi;
	//ZeroMemory((void*)&mi, sizeof(MONITORINFO));
	//mi.cbSize = sizeof(MONITORINFO);
	//code
	if (gbFullScreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			ZeroMemory((void*)&mi, sizeof(MONITORINFO));
			mi.cbSize = sizeof(MONITORINFO);
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP,
					mi.rcMonitor.left,
					mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
	}
	else
	{
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
}

int initilize(void)
{
	// function declaration
	void printGLInfo(void);
	void resize(int, int);

	// Varible declration
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	// code
	
	// Pixel format discripter Initilization
	ZeroMemory((void*)&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	// GetDC
	ghdc = GetDC(ghwnd);
	if (ghdc == NULL)
	{
		fprintf(gpFile, "GetDC function failed !! \n");
		return -1;
	}
	// get matching pixel index using hdc & 
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0)
	{
		fprintf(gpFile, "iPixelFormatIndex failed !! \n");
		return -2;
	}

	// Select the pixel format of found index
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFile, "SetPixelFormat function failed !! \n");
		return -3;
	}

	//create rendering contex using HDC, pfd and chosen PixelFormatIndex 
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		fprintf(gpFile, "wglCreateContext function failed !! \n");
		return -4;
	}

	// make this rendering contex as current context 
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFile, "wglMakeCurrent function failed !! \n");
		return -5;
	}
	
	// print GL info
	printGLInfo();
	// from here Open-GL code start

	// Depth related code
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// tail OpenGL to choose the color to clear the cscreen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// warmup resize
	resize(WIN_WIDTH, WIN_HEIGHT);

	return 0;
}

void printGLInfo(void)
{
	// code
	// print GL info
	fprintf(gpFile, "OPENGL INFORMATION \n");
	fprintf(gpFile, "**************** \n");
	fprintf(gpFile, "OpenGL Vendor   : %s \n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer : %s \n", glGetString(GL_RENDERER));
	fprintf(gpFile, "OpenGL Version  : %s \n", glGetString(GL_VERSION));
	fprintf(gpFile, "**************** \n");

}
void resize(int width, int haight)
{
	// code
	if (haight <= 0)
	{
		haight = 1;
	}
	// set the view port
	glViewport(0, 0, (GLsizei)width, (GLsizei)haight);

	// SetMatricsProjectionMode
	glMatrixMode(GL_PROJECTION);

	// Set to Identity Matrics
	glLoadIdentity();

	// Do Perspective Projection
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)haight, 0.1f, 100.0f);
	
}

void display(void)
{
	// code

	// Clear open GL Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set matrics to model view mode
	glMatrixMode(GL_MODELVIEW);

	// TRANGLE
	
	// Set to identity mode
	glLoadIdentity();

	// Translate trangle backword by - Z
	glTranslatef(0.0f, 0.0f, -6.0f);

	glRotatef(angleCube, 1.0f, 0.0f, 0.0f);

	glRotatef(angleCube, 0.0f, 1.0f, 0.0f);

	glRotatef(angleCube, 0.0f, 0.0f, 1.0f);

	
	// trangle drawing code
	glBegin(GL_QUADS);

    // front face
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Right Face
	glVertex3f(1.0f, 1.0f, -1.0f);  //top
	glVertex3f(1.0f, 1.0f, 1.0f);   //left top
	glVertex3f(1.0f, -1.0f, 1.0f);  //left bottom
	glVertex3f(1.0f, -1.0f, -1.0f);  //Right bottom

	// Back face
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	// Left Face 
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// top face 
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom face 
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glEnd();


	// Swap the buffers
	SwapBuffers(ghdc);
}

void update(void)
{
	// code
	angleCube = angleCube + 0.05f;

	if (angleCube >= 360.0f)
	{
		angleCube = angleCube - 360.0f;
	}
}

void uninitilize(void)
{
	// function declartiion
	void toggleFullScreen(void);

	// code
	// if exiting in fullscreen then restore Fullscreen first 
	if (gbFullScreen == TRUE)
	{
		toggleFullScreen();
		gbFullScreen = FALSE;
	}
	// make hdc as current context by release rendering context as current context
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}
	// Delete the rendering context 
	if (ghrc)
	{
		wglDeleteContext(ghrc); 
		ghrc = NULL;
	}
	// Release the DC
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}
	// Distroy window
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}
	// Close the file
	if (gpFile)
	{
		fprintf(gpFile, "Program Termineted Sucessfully !!! \n");
		fclose(gpFile);
		gpFile = NULL;
	}
}
