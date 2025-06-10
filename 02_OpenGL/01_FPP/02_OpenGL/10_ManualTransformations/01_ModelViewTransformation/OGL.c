// Win32 Headers files
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES 1
#include <math.h>

// OpenGl related header files
#include <gl/GL.h>
#include <gl/GLU.h>

// User defined header file
#include "OGL.h"

// OpenGl related libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GLU32.lib")

// Macros 
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable declarations
// variables related to FullScreen
BOOL gbFullScreen = FALSE;
HWND ghwnd = NULL;
DWORD dwStyle;
WINDOWPLACEMENT wpPrev;

// variables related with file I/O
char gszLogFileName[] = "Log.txt";
FILE* gpFile = NULL;

// Active window related variables
BOOL gbActiveWindow = FALSE;

// Exit Keypress related
BOOL gbEscapeKeyIsPressed = FALSE;

// OpenGl related Global variables
HDC ghdc = NULL;
HGLRC ghrc = NULL; // global handle to rendering context

// Rotaion Angles
float angleCube = 0.0f;

//
float identityMatrix[16];
float translateMatrix[16];
float scaleMatrix[16];
float rotationMatrixX[16];
float rotationMatrixY[16];
float rotationMatrixZ[16];

// Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Function Declarations
	int initialize(void);
	void display(void);
	void update(void);
	void uninitialize(void);

	// Variable Declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("RTR6");
	BOOL bDone = FALSE;

	// Code
	// Create Log File
	gpFile = fopen(gszLogFileName, "w");

	if (gpFile == NULL)
	{
		MessageBox(NULL, TEXT("LOG FILE CREATION FAILED !!!"), TEXT("FILE I/O ERROR"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Program started successfully\n");
	}

	// Window Class Intialization
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Registration of Window Class
	RegisterClassEx(&wndclass);

	// Create Window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Gaurav Kumar"),
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

	// Paint Background of the Window
	UpdateWindow(hwnd);

	// Message loop
	/*while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	*/

	// initialize
	int result = initialize();

	if (result != 0)
	{
		fprintf(gpFile, "Initialise() Failed !!!\n");
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
	else
	{
		fprintf(gpFile, "Initialise() Completed Successfully\n");
	}

	// set this window as foreground and active window
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// Game Loop
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
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
				if (gbEscapeKeyIsPressed == TRUE)
				{
					bDone = TRUE;
				}
				// Render
				display();

				// update
				update();
			}
		}
	}

	uninitialize();
	return((int)msg.wParam);
}

// Callback Function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declarations
	void toggleFullScreen(void);
	void resize(int, int);
	void uninitialize(void);

	// Code
	switch (iMsg)
	{
	case WM_CREATE:
		ZeroMemory((void*)&wpPrev, sizeof(WINDOWPLACEMENT));
		wpPrev.length = sizeof(WINDOWPLACEMENT);
		break;
	case WM_SETFOCUS:
		gbActiveWindow = TRUE;
		break;
	case WM_KILLFOCUS:
		gbActiveWindow = FALSE;
		break;
	case WM_ERASEBKGND:
		return(0); // flikker free rendering
	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE: // virtual key code 
			gbEscapeKeyIsPressed = TRUE;
			break;
		default:
			break;
		}
		break;
	case WM_CHAR:
		switch (wParam)
		{
		case 'f':
		case 'F':
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
		}
		break;
	case WM_CLOSE:
		uninitialize();
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
	// variable declarations
	MONITORINFO mi;

	// code 
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
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE); // optional for full screen
	}
	else
	{
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
}

int initialize(void)
{
	// Function declarations
	void printGLInfo(void);
	void resize(int, int);

	// variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	// code
	// PIXELFORMATDESCRIPTOR initialization
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

	// getDC
	ghdc = GetDC(ghwnd);
	if (ghdc == NULL)
	{
		fprintf(gpFile, "GetDC() failed !!!\n");
		return(-1);
	}

	// Get Matching Pixel format index using HDC and PFD
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);

	if (iPixelFormatIndex == 0)
	{
		fprintf(gpFile, "ChoosePixelFormat() failed !!!\n");
		return(-2);
	}

	// select the pixel format of found index
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFile, "SetPixelFormat() failed !!!\n");
		return(-3);
	}

	// create rendering context using hdc, pfd and chosen iPixelFormatIndex
	ghrc = wglCreateContext(ghdc);

	if (ghrc == NULL)
	{
		fprintf(gpFile, "wglCreateContext() failed !!!\n");
		return(-4);
	}

	// Make this rendering context as current context
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFile, "wglMakeCurrent() failed !!!\n");
		return(-5);
	}

	printGLInfo();

	// Depth related code
	glShadeModel(GL_SMOOTH);

	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// From here onwards OpenGL code starts
	// Tell OpenGL to choose the color to clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// define matrices
	// identity matrix
	identityMatrix[0] = 1.0f;
	identityMatrix[1] = 0.0f;
	identityMatrix[2] = 0.0f;
	identityMatrix[3] = 0.0f;
	identityMatrix[4] = 0.0f;
	identityMatrix[5] = 1.0f;
	identityMatrix[6] = 0.0f;
	identityMatrix[7] = 0.0f;
	identityMatrix[8] = 0.0f;
	identityMatrix[9] = 0.0f;
	identityMatrix[10] = 1.0f;
	identityMatrix[11] = 0.0f;
	identityMatrix[12] = 0.0f;
	identityMatrix[13] = 0.0f;
	identityMatrix[14] = 0.0f;
	identityMatrix[15] = 1.0f;

	// Translation Matrix
	translateMatrix[0] = 1.0f;
	translateMatrix[1] = 0.0f;
	translateMatrix[2] = 0.0f;
	translateMatrix[3] = 0.0f;
	translateMatrix[4] = 0.0f;
	translateMatrix[5] = 1.0f;
	translateMatrix[6] = 0.0f;
	translateMatrix[7] = 0.0f;
	translateMatrix[8] = 0.0f;
	translateMatrix[9] = 0.0f;
	translateMatrix[10] = 1.0f;
	translateMatrix[11] = 0.0f;
	translateMatrix[12] = 0.0f; // glTranslateMatrix ka X axis
	translateMatrix[13] = 0.0f; // glTranslateMatrix ka Y axis
	translateMatrix[14] = -6.0f; // glTranslateMatrix ka Z axis
	translateMatrix[15] = 1.0f;

	// Scale Matrix
	scaleMatrix[0] = 0.75f;
	scaleMatrix[1] = 0.0f;
	scaleMatrix[2] = 0.0f;
	scaleMatrix[3] = 0.0f;
	scaleMatrix[4] = 0.0f;
	scaleMatrix[5] = 0.75f;
	scaleMatrix[6] = 0.0f;
	scaleMatrix[7] = 0.0f;
	scaleMatrix[8] = 0.0f;
	scaleMatrix[9] = 0.0f;
	scaleMatrix[10] = 0.75f;
	scaleMatrix[11] = 0.0f;
	scaleMatrix[12] = 0.0f;
	scaleMatrix[13] = 0.0f;
	scaleMatrix[14] = 0.0f;
	scaleMatrix[15] = 1.0f;

	// Warmup resize
	resize(WIN_WIDTH, WIN_HEIGHT);
	return(0);
}

void printGLInfo(void)
{
	// code
	// print OpenGL Information
	fprintf(gpFile, "OPENGL INFORMATION\n");
	fprintf(gpFile, "******************\n");
	fprintf(gpFile, "OpenGL Vendor   : %s\n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer : %s\n", glGetString(GL_RENDERER));
	fprintf(gpFile, "OpenGL Version  : %s\n", glGetString(GL_VERSION));
	fprintf(gpFile, "******************\n");
}

void resize(int width, int height)
{
	// code
	// If height by accident become less or equal to zero then set it to 1
	if (height <= 0)
	{
		height = 1;
	}
	// set the view port
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	// Set matrix projection mode
	glMatrixMode(GL_PROJECTION);

	// Set to identity matrix
	glLoadIdentity();

	// Do Perspective projection
	gluPerspective(45.0f, // FOV-Y 
		(GLfloat)width / (GLfloat)height, // Aspect Ratio 
		0.1f, // Near 
		100.0f); // Far
}

void display(void)
{
	// variable declaration
	float angle = 0.0f;

	// code
	// Clear OpenGl Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TRIANGLE

	// Set Matrix to model view mode
	glMatrixMode(GL_MODELVIEW);

	// Set to identity matirx
	//glLoadIdentity();
	glLoadMatrixf(identityMatrix);

	// Translate Triangle Backwards by Z (-ve)
	//glTranslatef(0.0f, 0.0f, -6.0f);
	glMultMatrixf(translateMatrix);
	
	// Scaling
	// glscalef(0.75f, 0.75f, 0.75f);
	glMultMatrixf(scaleMatrix);

	// Rotation
	//glRotatef(angleCube, 1.0f, 0.0f, 0.0f);
	//glRotatef(angleCube, 0.0f, 1.0f, 0.0f);
	//glRotatef(angleCube, 0.0f, 0.0f, 1.0f);
	
	angle = angleCube * (M_PI/180.0f);
	// rotation matrix X
	rotationMatrixX[0] = 1.0f;
	rotationMatrixX[1] = 0.0f;
	rotationMatrixX[2] = 0.0f;
	rotationMatrixX[3] = 0.0f;
	rotationMatrixX[4] = 0.0f;
	rotationMatrixX[5] = cos(angle);
	rotationMatrixX[6] = sin(angle);
	rotationMatrixX[7] = 0.0f;
	rotationMatrixX[8] = 0.0f;
	rotationMatrixX[9] = -sin(angle);
	rotationMatrixX[10] = cos(angle);
	rotationMatrixX[11] = 0.0f;
	rotationMatrixX[12] = 0.0f;
	rotationMatrixX[13] = 0.0f;
	rotationMatrixX[14] = 0.0f;
	rotationMatrixX[15] = 1.0f;
	glMultMatrixf(rotationMatrixX);

	// rotation martix Y
	rotationMatrixY[0] = cos(angle);
	rotationMatrixY[1] = 0.0f;
	rotationMatrixY[2] = -sin(angle);
	rotationMatrixY[3] = 0.0f;
	rotationMatrixY[4] = 0.0f;
	rotationMatrixY[5] = 1.0f;
	rotationMatrixY[6] = 0.0f;
	rotationMatrixY[7] = 0.0f;
	rotationMatrixY[8] = sin(angle);
	rotationMatrixY[9] = 0.0f;
	rotationMatrixY[10] = cos(angle);
	rotationMatrixY[11] = 0.0f;
	rotationMatrixY[12] = 0.0f;
	rotationMatrixY[13] = 0.0f;
	rotationMatrixY[14] = 0.0f;
	rotationMatrixY[15] = 1.0f;
	glMultMatrixf(rotationMatrixY);

	// rotation matrix Z
	rotationMatrixZ[0] = cos(angle);
	rotationMatrixZ[1] = sin(angle);
	rotationMatrixZ[2] = 0.0f;
	rotationMatrixZ[3] = 0.0f;
	rotationMatrixZ[4] = -sin(angle);
	rotationMatrixZ[5] = cos(angle);
	rotationMatrixZ[6] = 0.0f;
	rotationMatrixZ[7] = 0.0f;
	rotationMatrixZ[8] = 0.0f;
	rotationMatrixZ[9] = 0.0f;
	rotationMatrixZ[10] = 1.0f;
	rotationMatrixZ[11] = 0.0f;
	rotationMatrixZ[12] = 0.0f;
	rotationMatrixZ[13] = 0.0f;
	rotationMatrixZ[14] = 0.0f;
	rotationMatrixZ[15] = 1.0f;
	glMultMatrixf(rotationMatrixZ);

	// Triangle drawing code
	glBegin(GL_QUADS);

	// Front Face
	glColor3f(1.0f, 0.0f, 0.0f);
	
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Right Face
	glColor3f(0.0f, 1.0f, 0.0f);
	
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Back Face
	glColor3f(0.0f, 0.0f, 1.0f);
	
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	// Left Face
	glColor3f(0.0f, 1.0f, 1.0f);

	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Top Face
	glColor3f(1.0f, 0.0f, 1.0f);

	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom Face
	glColor3f(1.0f, 1.0f, 0.0f);

	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glEnd();

	// Swap the Buffers
	SwapBuffers(ghdc);
}

void update(void)
{
	// code
	angleCube = angleCube + 0.5f;

	if (angleCube >= 360.0f)
	{
		angleCube = angleCube - 360.0f;
	}
}

void uninitialize(void)
{
	// function declarations
	void toggleFullScreen(void);

	// code
	// If user is exiting in FullScreen, then restore it to normal window 
	if (gbFullScreen == TRUE)
	{
		toggleFullScreen();
		gbFullScreen = FALSE;
	}

	// Make HDC as current context by releasing rendering context as current context
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	// delete the rendering context
	if (ghrc != NULL)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	// release the DC
	if (ghdc != NULL)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	// Destroy Window
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}

	// close the file
	if (gpFile != NULL)
	{
		fprintf(gpFile, "Program terminated successfully\n");
		fclose(gpFile);
		gpFile = NULL;
	}
}
