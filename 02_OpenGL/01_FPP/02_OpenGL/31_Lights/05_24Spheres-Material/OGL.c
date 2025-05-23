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

GLUquadric *qudric = NULL;

// variables for 24 sphere
BOOL bLight = FALSE;
GLfloat lightAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightDifused[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPosition[] = {0.0f, 0.0f, 0.0f, 1.0f};

GLfloat lightModelAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat lightModelLocalViewer[] = {0.0f};

GLfloat angleForXRotatio = 0.0f;
GLfloat angleForYRotatio = 0.0f;
GLfloat angleForZRotatio = 0.0f;

GLint keyPressed = -1;

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

		TEXT("GAURAV"),
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
		case 'L':
		case 'l':
			if (bLight == FALSE)
			{
				bLight = TRUE;
				glEnable(GL_LIGHTING);
			}
			else
			{
				bLight = FALSE;
				glDisable(GL_LIGHTING);
			}
			break;
		case 'X':
		case 'x':
			keyPressed = 1;
			angleForXRotatio = 0.0f; // reseting of angle
			break;
		case 'Y':
		case 'y':
			keyPressed = 2;
			angleForYRotatio = 0.0f; // reseting of angle
			break;
		case 'Z':
		case 'z':
			keyPressed = 3;
			angleForZRotatio = 0.0f; // reseting of angle
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
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// intilize qudric
	qudric = gluNewQuadric();     // give memory

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Light 0 configuration
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifused);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glEnable(GL_LIGHT0);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, lightModelLocalViewer);

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

void resize(int width, int height)
{
	// code
	if (height <= 0)
	{
		height = 1;
	}
	// set the view port
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	// SetMatricsProjectionMode
	glMatrixMode(GL_PROJECTION);

	// Set to Identity Matrics
	glLoadIdentity();

	// Do Perspective Projection
	//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	// Do orthographic projection
	if (width <= height)
	{
		glOrtho(0.0f, //  left
			15.5f, // right
			(0.0f * ((GLfloat)height / (GLfloat)width)), // bottom
			(15.5f * ((GLfloat)height / (GLfloat)width)), // top
			-10.0f,  // near
			10.0f); // far
	}
	else
	{
		glOrtho((0.0f * ((GLfloat)width / (GLfloat)height)), // left
			(15.5f * ((GLfloat)width / (GLfloat)height)), // right
			0.0f, // bottom
			15.5f, // top
			-10.0f, // near
			10.0f); // far
	}
}

void display(void)
{
	// function declarations
	void draw24Spheres(void);

	// code
	// Clear open GL Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set matrics to model view mode
	glMatrixMode(GL_MODELVIEW);

	// Set to identity mode
	glLoadIdentity();

	if (keyPressed == 1)
	{
		glRotatef(angleForXRotatio, 1.0f, 0.0f, 0.0f);
		lightPosition[2] = angleForXRotatio;
	}
	else if (keyPressed == 2)
	{
		glRotatef(angleForYRotatio, 0.0f, 1.0f, 0.0f);
		lightPosition[0] = angleForYRotatio;
	}
	else if (keyPressed == 3)
	{
		glRotatef(angleForZRotatio, 0.0f, 0.0f, 1.0f);
		lightPosition[1] = angleForZRotatio;
	}
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	// draw24Spheres
	draw24Spheres();

	// Swap the buffers
	SwapBuffers(ghdc);
}

void draw24Spheres(void)
{
	// variable declarations
	GLfloat materialAmbient[4];
	GLfloat materialDifused[4];
	GLfloat materialSpecular[4];
	GLfloat materialShininess;

	//code
	// 1st Column
	// 1st sphere of 1st column
	materialAmbient[0] = 0.0215f;
	materialAmbient[1] = 0.1745f;
	materialAmbient[2] = 0.0215f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.07568f;
	materialDifused[1] = 0.61424f;
	materialDifused[2] = 0.07568f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);
	
	// specular 
	materialSpecular[0] = 0.633f;
	materialSpecular[1] = 0.727811f;
	materialSpecular[2] = 0.633f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.6f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f, 14.0f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 2nd sphere of 1st coloumn
	materialAmbient[0] = 0.135f;
	materialAmbient[1] = 0.2225f;
	materialAmbient[2] = 0.1575f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.54f;
	materialDifused[1] = 0.89f;
	materialDifused[2] = 0.63f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.316228f;
	materialSpecular[1] = 0.316228f;
	materialSpecular[2] = 0.316228f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.1f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f, 11.5f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 3nd sphere of 1st coloumn
	materialAmbient[0] = 0.05375f;
	materialAmbient[1] = 0.05f;
	materialAmbient[2] = 0.06625f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.18275f;
	materialDifused[1] = 0.17f;
	materialDifused[2] = 0.22525f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.332741f;
	materialSpecular[1] = 0.328634f;
	materialSpecular[2] = 0.346435f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.3f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f, 9.0f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 4th sphere of 1st coloumn
	materialAmbient[0] = 0.25f;
	materialAmbient[1] = 0.20725f;
	materialAmbient[2] = 0.20725f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 1.0f;
	materialDifused[1] = 0.829f;
	materialDifused[2] = 0.829f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.296648f;
	materialSpecular[1] = 0.296648f;
	materialSpecular[2] = 0.296648f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.088f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f, 6.5f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 5th sphere of 1st coloumn
	materialAmbient[0] = 0.1745f;
	materialAmbient[1] = 0.01175f;
	materialAmbient[2] = 0.01175f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.61424f;
	materialDifused[1] = 0.04136f;
	materialDifused[2] = 0.04136f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.727811f;
	materialSpecular[1] = 0.626959f;
	materialSpecular[2] = 0.626959f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.6f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f, 4.0f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 6th sphere of 1st coloumn
	materialAmbient[0] = 0.1f;
	materialAmbient[1] = 0.18725f;
	materialAmbient[2] = 0.1745f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.396f;
	materialDifused[1] = 0.74151f;
	materialDifused[2] = 0.69102f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.297254f;
	materialSpecular[1] = 0.30829f;
	materialSpecular[2] = 0.306678f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.1f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f, 1.5f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);
	
	// 2nd Column *************************************************************
	// 1st sphere of 2nd coloumn
	materialAmbient[0] = 0.329412f;
	materialAmbient[1] = 0.223529f;
	materialAmbient[2] = 0.027451f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.780392f;
	materialDifused[1] = 0.568627f;
	materialDifused[2] = 0.113725f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.992157f;
	materialSpecular[1] = 0.941176f;
	materialSpecular[2] = 0.807843;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.21794872f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(7.5f, 14.0f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 2nd sphere of 2nd coloumn
	materialAmbient[0] = 0.2125f;
	materialAmbient[1] = 0.1275f;
	materialAmbient[2] = 0.054f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.714f;
	materialDifused[1] = 0.4284f;
	materialDifused[2] = 0.18144f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.393548f;
	materialSpecular[1] = 0.271906f;
	materialSpecular[2] = 0.166721f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.2f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(7.5f, 11.5f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 3nd sphere of 2nd coloumn
	materialAmbient[0] = 0.25f;
	materialAmbient[1] = 0.25f;
	materialAmbient[2] = 0.25f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.4f;
	materialDifused[1] = 0.4f;
	materialDifused[2] = 0.4f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.774597f;
	materialSpecular[1] = 0.774597f;
	materialSpecular[2] = 0.774597f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.6f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(7.5f, 9.0f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 4th sphere of 2nd coloumn
	materialAmbient[0] = 0.19125f;
	materialAmbient[1] = 0.0735f;
	materialAmbient[2] = 0.0225f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.7038f;
	materialDifused[1] = 0.27048f;
	materialDifused[2] = 0.0828f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.256777f;
	materialSpecular[1] = 0.137622f;
	materialSpecular[2] = 0.086014f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.1f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(7.5f, 6.5f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 5th sphere of 2nd coloumn
	materialAmbient[0] = 0.24725f;
	materialAmbient[1] = 0.1995f;
	materialAmbient[2] = 0.0745f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.75164f;
	materialDifused[1] = 0.60648f;
	materialDifused[2] = 0.22648f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.628281f;
	materialSpecular[1] = 0.555802f;
	materialSpecular[2] = 0.366065f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.4f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(7.5f, 4.0f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 6th sphere of 2nd coloumn
	materialAmbient[0] = 0.19225f;
	materialAmbient[1] = 0.19225f;
	materialAmbient[2] = 0.19225f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.50754f;
	materialDifused[1] = 0.50754f;
	materialDifused[2] = 0.50754f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.508273f;
	materialSpecular[1] = 0.508273f;
	materialSpecular[2] = 0.508273f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.4f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(7.5f, 1.5f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 3rd Column *****************************************************
	// 1st sphere of 3rd coloumn
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.01f;
	materialDifused[1] = 0.01f;
	materialDifused[2] = 0.01f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.50f;
	materialSpecular[1] = 0.50f;
	materialSpecular[2] = 0.50f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(13.5f, 14.0f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 2nd sphere of 3rd coloumn
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.1f;
	materialAmbient[2] = 0.06f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.0f;
	materialDifused[1] = 0.50980392f;
	materialDifused[2] = 0.50980392f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.50196078f;
	materialSpecular[1] = 0.50196078f;
	materialSpecular[2] = 0.50196078f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(13.5f, 11.5f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 3nd sphere of 3rd coloumn
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.1f;
	materialDifused[1] = 0.35f;
	materialDifused[2] = 0.1f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.45f;
	materialSpecular[1] = 0.55f;
	materialSpecular[2] = 0.45f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(13.5f, 9.0f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 4th sphere of 3rd coloumn
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.5f;
	materialDifused[1] = 0.0f;
	materialDifused[2] = 0.0f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.7f;
	materialSpecular[1] = 0.6f;
	materialSpecular[2] = 0.6f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(13.5f, 6.5f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 5th sphere of 3rd coloumn
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.55f;
	materialDifused[1] = 0.55f;
	materialDifused[2] = 0.55f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.70f;
	materialSpecular[1] = 0.70f;
	materialSpecular[2] = 0.70f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(13.5f, 4.0f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 6th sphere of 3rd coloumn
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.5f;
	materialDifused[1] = 0.5f;
	materialDifused[2] = 0.0f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.60f;
	materialSpecular[1] = 0.60f;
	materialSpecular[2] = 0.50f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(13.5f, 1.5f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 4th Column *************************************************************
	// 1st sphere of 4th coloumn
	materialAmbient[0] = 0.02f;
	materialAmbient[1] = 0.02f;
	materialAmbient[2] = 0.02f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.01f;
	materialDifused[1] = 0.01f;
	materialDifused[2] = 0.01f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.4f;
	materialSpecular[1] = 0.4f;
	materialSpecular[2] = 0.4f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(19.5f, 14.0f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 2nd sphere of 4th coloumn
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.05f;
	materialAmbient[2] = 0.05f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.4f;
	materialDifused[1] = 0.5f;
	materialDifused[2] = 0.5f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.04f;
	materialSpecular[1] = 0.7f;
	materialSpecular[2] = 0.7f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(19.5f, 11.5f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 3nd sphere of 4th coloumn
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.05f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.4f;
	materialDifused[1] = 0.5f;
	materialDifused[2] = 0.4f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.04f;
	materialSpecular[1] = 0.7f;
	materialSpecular[2] = 0.04f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(19.5f, 9.0f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 4th sphere of 4th coloumn
	materialAmbient[0] = 0.05f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.5f;
	materialDifused[1] = 0.4f;
	materialDifused[2] = 0.4f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.7f;
	materialSpecular[1] = 0.04f;
	materialSpecular[2] = 0.04f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(19.5f, 6.5f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 5th sphere of 4th coloumn
	materialAmbient[0] = 0.05f;
	materialAmbient[1] = 0.05f;
	materialAmbient[2] = 0.05f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.5f;
	materialDifused[1] = 0.5f;
	materialDifused[2] = 0.5f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.7f;
	materialSpecular[1] = 0.7f;
	materialSpecular[2] = 0.7f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(19.5f, 4.0f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);

	// 6th sphere of 4th coloumn
	materialAmbient[0] = 0.05;
	materialAmbient[1] = 0.05f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// Difused
	materialDifused[0] = 0.5f;
	materialDifused[1] = 0.5f;
	materialDifused[2] = 0.4f;
	materialDifused[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifused);

	// specular 
	materialSpecular[0] = 0.7f;
	materialSpecular[1] = 0.7f;
	materialSpecular[2] = 0.04f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(19.5f, 1.5f, 0.0f);
	gluSphere(qudric, 1.0f, 30, 30);
}

void update(void)
{
	// code
	angleForXRotatio += 0.05f;
	if (angleForXRotatio >= 360.0f)
	{
		angleForXRotatio = angleForXRotatio - 360.0f;
	}

	angleForYRotatio += 0.05f;
	if (angleForYRotatio >= 360.0f)
	{
		angleForYRotatio = angleForYRotatio - 360.0f;
	}

	angleForZRotatio += 0.05f;
	if (angleForZRotatio >= 360.0f)
	{
		angleForZRotatio = angleForZRotatio - 360.0f;
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

	if (qudric)
	{
		gluDeleteQuadric(qudric);
		qudric = NULL;
	}

	// Close the file
	if (gpFile)
	{
		fprintf(gpFile, "Program Termineted Sucessfully !!! \n");
		fclose(gpFile);
		gpFile = NULL;
	}
}
