// Win32 Headers files
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

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
FILE *gpFile = NULL;

// Active window related variables
BOOL gbActiveWindow = FALSE;

// Exit Keypress related
BOOL gbEscapeKeyIsPressed = FALSE;

// OpenGl related Global variables
HDC ghdc = NULL;
HGLRC ghrc = NULL; // global handle to rendering context

// My Global Variables
float o[3] = {1.0f, 0.6f, 0.2f};
float w[3] = {1.0f, 1.0f, 1.0f};
float g[3] = {0.0745f, 0.5333f, 0.0314f};

// variables related to Dynamic Bharat
BOOL bDrawH = FALSE;
BOOL bDrawA = FALSE;
BOOL bDrawR = FALSE;
BOOL bDrawT = FALSE;
BOOL bDrawA1 = FALSE;

GLfloat g_MoveB = -1.0f;
GLfloat g_MoveHx = -1.0f;
GLfloat g_MoveHy = 1.0f;

GLfloat g_MoveAx = -1.0f;
GLfloat g_MoveAy = -1.0f;

GLfloat g_MoveRx = 1.0f;
GLfloat g_MoveRy = 1.0f;

GLfloat g_MoveTx = 1.0f;
//GLfloat g_MoveTy = 1.0f;

GLfloat g_MoveA1x = 1.0f;
GLfloat g_MoveA1y = -1.0f;

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

	if(gpFile == NULL)
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

	if(result != 0)
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
	while(bDone == FALSE)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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
			if(gbActiveWindow == TRUE)
			{
				if(gbEscapeKeyIsPressed == TRUE)
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
	switch(iMsg)
	{
		case WM_CREATE:
			ZeroMemory((void *)&wpPrev, sizeof(WINDOWPLACEMENT));
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
			switch(wParam)
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
			ZeroMemory((void *)&mi, sizeof(MONITORINFO));
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
	if(SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFile, "SetPixelFormat() failed !!!\n");
		return(-3);
	}

	// create rendering context using hdc, pfd and chosen iPixelFormatIndex
	ghrc = wglCreateContext(ghdc);
	
	if(ghrc == NULL)
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
		(GLfloat) width / (GLfloat)height, // Aspect Ratio 
		0.1f, // Near 
		100.0f); // Far
}

void display(void)
{
	// Function declarations
	void B(void);
	void H(void);
	void A(void);
	void R(void);
	void T(void);
	void A1(void);
	
	// code
	// Clear OpenGl Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set Matrix to model view mode
	glMatrixMode(GL_MODELVIEW);
	
	// Set to identity matirx
	//glLoadIdentity();

	// Transla7te Triangle Backwards by Z (-ve)
	//glTranslatef(0.1f, -0.3f, -2.0f);

	// BHARAT
	B();

	if(bDrawH)
		H();
	
	if(bDrawA)
		A();
	
	if(bDrawR)
		R();
	
	if(bDrawA1)
		A1();
	
	if(bDrawT)
		T();

	// Swap the Buffers
	SwapBuffers(ghdc);
}

void update(void)
{
	// code
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
	if(gpFile != NULL)
	{
		fprintf(gpFile, "Program terminated successfully\n");
		fclose(gpFile);
		gpFile = NULL;
	}
}

// B
void B(void)
{
	glLoadIdentity();
	glTranslatef(g_MoveB, -0.3f, -2.0f);

	glBegin(GL_QUADS);
	
	// Line 1

	glColor3f(o[0], o[1], o[2]);
	glVertex3f(-1.0f, 0.55f, 0.0f);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-1.0f, 0.35f, 0.0f);
	glVertex3f(-0.95f, 0.35f, 0.0f);
	glColor3f(o[0], o[1], o[2]);
	glVertex3f(-0.95f, 0.55f, 0.0f);
	
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-1.0f, 0.35f, 0.0f);
	glVertex3f(-1.0f, 0.2f, 0.0f);
	glVertex3f(-0.95f, 0.2f, 0.0f);
	glVertex3f(-0.95f, 0.35f, 0.0f);

	glVertex3f(-1.0f, 0.2f, 0.0f);
	glColor3f(g[0], g[1], g[2]);
	glVertex3f(-1.0f, 0.05, 0.0f);
	glVertex3f(-0.95f, 0.05f, 0.0f);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-0.95f, 0.2f, 0.0f);

	glColor3f(o[0], o[1], o[2]);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glVertex3f(-1.0f, 0.55f, 0.0f);
	glVertex3f(-0.85f, 0.55f, 0.0f);
	glVertex3f(-0.8f, 0.6f, 0.0f);

	glColor3f(o[0], o[1], o[2]);
	glVertex3f(-0.85f, 0.54f, 0.0f);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-0.85f, 0.27f, 0.0f);
	glVertex3f(-0.8f,  0.32f, 0.0f);
	glColor3f(o[0], o[1], o[2]);
	glVertex3f(-0.8f,  0.59f, 0.0f);
	
	// middle
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-0.95f, 0.3f, 0.0f);
	glVertex3f(-0.95f, 0.25f, 0.0f);
	glVertex3f(-0.86f, 0.25f, 0.0f);
	glVertex3f(-0.86f, 0.3f, 0.0f);

	glColor3f(g[0], g[1], g[2]);
	glVertex3f(-0.8f, 0.01f, 0.0f);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-0.8f, 0.22f, 0.0f);
	glVertex3f(-0.85f,0.27f, 0.0f);
	glColor3f(g[0], g[1], g[2]);
	glVertex3f(-0.85f,0.055f, 0.0f);

	glColor3f(g[0], g[1], g[2]);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-0.804f,0.0f, 0.0f);
	glVertex3f(-0.86f, 0.05f, 0.0f);
	glVertex3f(-1.0f, 0.05f, 0.0f);
	glEnd();

	if (g_MoveB < 0.1f)
		g_MoveB = g_MoveB + 0.0001f;
	else
		bDrawH = TRUE;
}

// H
void H(void)
{
	glLoadIdentity();
	glTranslatef(g_MoveHx, g_MoveHy, -2.0f);

	//Middle
	glBegin(GL_QUADS);

	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-0.7f, 0.3f, 0.0f);
	glVertex3f(-0.7f, 0.25f, 0.0f);
	glVertex3f(-0.45f, 0.25f, 0.0f);
	glVertex3f(-0.45f, 0.3f, 0.0f);
	//glEnd();

	//glBegin(GL_QUADS);
	glColor3f(o[0], o[1], o[2]);
	glVertex3f(-0.7f, 0.6f, 0.0f);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-0.7f, 0.3f, 0.0f);
	glVertex3f(-0.65f, 0.3f, 0.0f);
	glColor3f(o[0], o[1], o[2]);
	glVertex3f(-0.65f, 0.6f, 0.0f);
	//glEnd();

	//glBegin(GL_QUADS);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-0.7f, 0.25f, 0.0f);
	glColor3f(g[0], g[1], g[2]);
	glVertex3f(-0.7f, 0.0f, 0.0f);
	glVertex3f(-0.65f, 0.0f, 0.0f);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-0.65f, 0.25f, 0.0f);
	//glEnd();

	//glBegin(GL_QUADS);
	glColor3f(o[0], o[1], o[2]);
	glVertex3f(-0.5f, 0.6f, 0.0f);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-0.5f, 0.3f, 0.0f);
	glVertex3f(-0.45f, 0.3f, 0.0f);
	glColor3f(o[0], o[1], o[2]);
	glVertex3f(-0.45f, 0.6f, 0.0f);
	//glEnd();

	//glBegin(GL_QUADS);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-0.5f, 0.25f, 0.0f);
	glColor3f(g[0], g[1], g[2]);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(-0.45f, 0.0f, 0.0f);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-0.45f, 0.25f, 0.0f);
	glEnd();

	if (g_MoveHx < 0.05f)
	{
		g_MoveHx = g_MoveHx + 0.00021f;
	}
	if (g_MoveHy > -0.3f)
	{
		g_MoveHy = g_MoveHy - 0.00026f;
	}
		//fprintf(gpFile, "g_MoveHx = %f  g_MoveHy = %f \n", g_MoveHx, g_MoveHy);
	else
		bDrawA = TRUE;
}

// A
void A(void)
{
	glLoadIdentity();
	glTranslatef(g_MoveAx, g_MoveAy, -2.0f);

	// /
	glBegin(GL_QUADS);
	glColor3f(g[0], g[1], g[2]);
	glVertex3f(-0.4f, 0.0f, 0.0f);
	glVertex3f(-0.35f, 0.0f, 0.0f);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-0.3f, 0.25f, 0.0f);
	glVertex3f(-0.35f, 0.25f, 0.0f);

	glColor3f(o[0], o[1], o[2]);
	glVertex3f(-0.25f, 0.6f, 0.0f);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-0.34f, 0.3f, 0.0f);
	glVertex3f(-0.29f, 0.3f, 0.0f);
	glColor3f(o[0], o[1], o[2]);
	glVertex3f(-0.25f, 0.5f, 0.0f);

	//Middle -

	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-0.12f, 0.3f, 0.0f);
	glVertex3f(-0.38f, 0.3f, 0.0f);
	glVertex3f(-0.36f, 0.25f, 0.0f);
	glVertex3f(-0.14f, 0.25f, 0.0f);

	// '\'
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-0.15f, 0.25f, 0.0f);
	glVertex3f(-0.2f, 0.25f, 0.0f);
	glColor3f(g[0], g[1], g[2]);
	glVertex3f(-0.15f, 0.0f, 0.0f);
	glVertex3f(-0.1f, 0.0f, 0.0f);

	// 
	glColor3f(o[0], o[1], o[2]);
	glVertex3f(-0.25f, 0.6f, 0.0f);
	glVertex3f(-0.25f, 0.5f, 0.0f);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(-0.21f, 0.3f, 0.0f);
	glVertex3f(-0.16f, 0.3f, 0.0f);

	glEnd();

	if (g_MoveAx < 0.05f)
	{
		g_MoveAx = g_MoveAx + 0.000221f;
	}
	if (g_MoveAy < -0.3f)
		g_MoveAy = g_MoveAy + 0.00015f;

	//fprintf(gpFile, "g_MoveHx = %f  g_MoveHy = %f \n", g_MoveHx, g_MoveHy);
	else
		bDrawR = TRUE;
}

// R
void R(void)
{
	glLoadIdentity();
	glTranslatef(g_MoveRx, g_MoveRy, -2.0f);

	glBegin(GL_QUADS);
		//
		glColor3f(w[0], w[1], w[2]);
		glVertex3f(0.0f, 0.25f, 0.0f);
		glVertex3f(-0.05f, 0.25f, 0.0f);
		glColor3f(g[0], g[1], g[2]);
		glVertex3f(-0.05f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
	
		// 
		glColor3f(o[0], o[1], o[2]);
		glVertex3f(0.0f, 0.6f, 0.0f);
		glVertex3f(-0.05f, 0.6f, 0.0f);
		glColor3f(w[0], w[1], w[2]);
		glVertex3f(-0.05f, 0.3f, 0.0f);
		glVertex3f(0.0f, 0.3f, 0.0f);

		//Middle -

		glColor3f(w[0], w[1], w[2]);
		glVertex3f(0.2f, 0.3f, 0.0f);
		glVertex3f(-0.05f, 0.3f, 0.0f);
		glVertex3f(-0.05f, 0.25f, 0.0f);
		glVertex3f(0.2f, 0.25f, 0.0f);

		//
		glColor3f(o[0], o[1], o[2]);
		glVertex3f(0.2f, 0.6f, 0.0f);
		glVertex3f(0.0f, 0.6f, 0.0f);
		glVertex3f(0.0f, 0.55f, 0.0f);
		glVertex3f(0.2f, 0.55f, 0.0f);

		//
		glColor3f(o[0], o[1], o[2]);
		glVertex3f(0.2f, 0.6f, 0.0f);
		glVertex3f(0.15f, 0.55f, 0.0f);
		glColor3f(w[0], w[1], w[2]);
		glVertex3f(0.15f, 0.3f, 0.0f);
		glVertex3f(0.2f, 0.3f, 0.0f);

		//
		glColor3f(w[0], w[1], w[2]);
		glVertex3f(0.05f, 0.25f, 0.0f);
		glVertex3f(0.0f, 0.25f, 0.0f);
		glColor3f(g[0], g[1], g[2]);
		glVertex3f(0.15f, 0.0f, 0.0f);
		glVertex3f(0.2f, 0.0f, 0.0f);

	glEnd();

	if (g_MoveRx > 0.05f)
	{
		g_MoveRx = g_MoveRx - 0.0002f;
	}
	if (g_MoveRy > -0.3f)
		g_MoveRy = g_MoveRy - 0.000271f;

	//fprintf(gpFile, "g_MoveHx = %f  g_MoveHy = %f \n", g_MoveHx, g_MoveHy);
	else
		bDrawA1 = TRUE;
}

// A1
void A1(void)
{
	glLoadIdentity();
	glTranslatef(g_MoveA1x, g_MoveA1y, -2.0f);

	// /
	glBegin(GL_QUADS);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(0.4f, 0.25f, 0.0f);
	glVertex3f(0.35f, 0.25f, 0.0f);
	glColor3f(g[0], g[1], g[2]);
	glVertex3f(0.3f, 0.0f, 0.0f);
	glVertex3f(0.35f, 0.0f, 0.0f);

	glColor3f(o[0], o[1], o[2]);
	glVertex3f(0.45f, 0.6f, 0.0f);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(0.36f, 0.3f, 0.0f);
	glVertex3f(0.41f, 0.3f, 0.0f);
	glColor3f(o[0], o[1], o[2]);
	glVertex3f(0.45f, 0.5f, 0.0f);

	//Middle -

	glColor3f(w[0], w[1], w[2]);
	glVertex3f(0.57f, 0.3f, 0.0f);
	glVertex3f(0.33f, 0.3f, 0.0f);
	glVertex3f(0.34f, 0.25f, 0.0f);
	glVertex3f(0.56f, 0.25f, 0.0f);

	// '\'
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(0.55f, 0.25f, 0.0f);
	glVertex3f(0.5f, 0.25f, 0.0f);
	glColor3f(g[0], g[1], g[2]);
	glVertex3f(0.55f, 0.0f, 0.0f);
	glVertex3f(0.6f, 0.0f, 0.0f);

	//
	glColor3f(o[0], o[1], o[2]);
	glVertex3f(0.45f, 0.6f, 0.0f);
	glVertex3f(0.45f, 0.5f, 0.0f);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(0.49f, 0.3f, 0.0f);
	glVertex3f(0.54f, 0.3f, 0.0f);

	glEnd();

	if (g_MoveA1x > 0.02f)
	{
		g_MoveA1x = g_MoveA1x - 0.00028f;
	}
	if (g_MoveA1y < -0.3f)
		g_MoveA1y = g_MoveA1y + 0.0002f;

	//fprintf(gpFile, "g_MoveHx = %f  g_MoveHy = %f \n", g_MoveHx, g_MoveHy);
	else
		bDrawT = TRUE;
}

// T
void T(void)
{
	glLoadIdentity();
	glTranslatef(g_MoveTx, -0.3f, -2.0f);

	glBegin(GL_QUADS);

	glColor3f(o[0], o[1], o[2]);
	glVertex3f(0.55f, 0.6f, 0.0f);
	glVertex3f(0.55f, 0.55f, 0.0f);
	glVertex3f(0.85f, 0.55f, 0.0f);
	glVertex3f(0.85f, 0.6f, 0.0f);
	//glEnd();

	//glBegin(GL_QUADS);
	glVertex3f(0.68f, 0.55f, 0.0f);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(0.68f, 0.35f, 0.0f);
	glVertex3f(0.72f, 0.35f, 0.0f);
	glColor3f(o[0], o[1], o[2]);
	glVertex3f(0.72f, 0.55f, 0.0f);
	//glEnd();

	//glBegin(GL_QUADS);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(0.68f, 0.35f, 0.0f);
	glVertex3f(0.68f, 0.25f, 0.0f);
	glVertex3f(0.72f, 0.25f, 0.0f);
	glVertex3f(0.72f, 0.35f, 0.0f);
	//glEnd();

	//glBegin(GL_QUADS);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(0.68f, 0.25f, 0.0f);
	glColor3f(g[0], g[1], g[2]);
	glVertex3f(0.68f, 0.0f, 0.0f);
	glVertex3f(0.72f, 0.0f, 0.0f);
	glColor3f(w[0], w[1], w[2]);
	glVertex3f(0.72f, 0.25f, 0.0f);
	glEnd();

	if (g_MoveTx > 0.02f)
	{
		g_MoveTx = g_MoveTx - 0.00028f;
	}
}
