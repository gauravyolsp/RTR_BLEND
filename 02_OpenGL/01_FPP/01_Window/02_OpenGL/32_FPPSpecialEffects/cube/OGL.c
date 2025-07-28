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

#define MAX_LIMIT_CUBE_TRANSLATE_X 8.0f
#define MIN_LIMIT_CUBE_TRANSLATE_X -8.0f

#define MAX_LIMIT_CUBE_TRANSLATE_Y 4.0f
#define MIN_LIMIT_CUBE_TRANSLATE_Y 0.45f

#define CLOSEST_LIMIT_CUBE_TRANSLATE_Z  5.0f
#define FARTHEST_LIMIT_CUBE_TRANSLATE_Z -20.0f

#define INCREMENT 0.1f
#define DECREMENT 0.1f

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

// Variables For Lights
GLfloat lightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat lightDiffused[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPosition[] = {0.0f, 5.0f, 10.0f, 1.0f};

GLfloat materialAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat materialDiffused[] = { 1.0f, 0.0f, 0.0f, 1.0f};
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialShininess = 128.0f;

// Special effect related varaibles
GLuint texture_marble = 0;

GLfloat translateCubeX = 0.0f;
GLfloat translateCubeY = 1.0f;
GLfloat translateCubeZ = 0.0f;

GLfloat cubeScale = 0.25f;

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

		TEXT("Gaurav"),
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
		case VK_RIGHT: // To translate cube on +ve x axis
			if (translateCubeX < MAX_LIMIT_CUBE_TRANSLATE_X)
			{
				translateCubeX += INCREMENT;
			}
			break;
		case VK_LEFT:// To translate cube on -ve x axis
			if (translateCubeX > MIN_LIMIT_CUBE_TRANSLATE_X)
			{
				translateCubeX -= DECREMENT;
			}
			break;
		case VK_UP:// To translate cube on +ve y axis
			if (translateCubeY < MAX_LIMIT_CUBE_TRANSLATE_Y)
			{
				translateCubeY += INCREMENT;
			}
			break;
		case VK_DOWN:// To translate cube on y axis (down)
			if (translateCubeY > MIN_LIMIT_CUBE_TRANSLATE_Y)
			{
				translateCubeY -= DECREMENT;
			}
			break;
		case VK_ADD: // towards the viewer
			if (translateCubeZ < CLOSEST_LIMIT_CUBE_TRANSLATE_Z)
			{
				translateCubeZ += INCREMENT;
			}
			break;
		case VK_SUBTRACT: // Away from viewer
			if (translateCubeZ > FARTHEST_LIMIT_CUBE_TRANSLATE_Z)
			{
				translateCubeZ -= DECREMENT;
			}
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
	BOOL loadGLTexture(GLuint*, TCHAR[]);

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

	// Without this line we don't have stencil buffer
	pfd.cStencilBits = 24;

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

	// Load Textures
	if (loadGLTexture(&texture_marble, MAKEINTRESOURCE(IDBITMAP_MARBLE)) == FALSE)
	{
		fprintf(gpFile, "Load Texture Failure for marble.bmp");
		return(-6);
	}

	// Enable Texturing
	glEnable(GL_TEXTURE_2D);

	// Light Configuaration
	glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffused);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT0);

	glMaterialfv(GL_FRONT, GL_AMBIENT,  materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,  materialDiffused);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT,  GL_SHININESS, materialShininess);

	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);

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

BOOL loadGLTexture(GLuint* texture, TCHAR imageRecourceID[])
{
	// VARIABLE DECLARATIONS	
	HBITMAP hBitMap = NULL;
	BITMAP bmp;
	BOOL bResult = FALSE;

	// CODE 
	// Load the bitmap as a image
	hBitMap = (HBITMAP)LoadImage(GetModuleHandleA(NULL), imageRecourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	if (hBitMap)
	{
		bResult = TRUE;

		// Get Bitmap structure from the loaded bitmap image
		GetObject(hBitMap, sizeof(BITMAP), &bmp);

		// Generate OpenGL Texture Object
		glGenTextures(1, texture);

		// Bind to the new created empty structure object
		glBindTexture(GL_TEXTURE_2D, *texture);

		// Unpack the image in memory for faster loading
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		//  Object near to eyes

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//  Object far to eyes
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// 64*64, 32*32 ... 1*1
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

		// Un-Bind 0 denotes unbind
		glBindTexture(GL_TEXTURE_2D, 0);

		DeleteObject(hBitMap);
		hBitMap = NULL;
	}

	return bResult;
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
	// function declaration
	void drawLitCube(void);
	void drawFloor(void);

	// code
	// Clear open GL Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// set matrics to model view mode
	glMatrixMode(GL_MODELVIEW);

	// CUBE
	
	// Set to identity mode
	glLoadIdentity();

	// set up camera
	gluLookAt(0.0f, 5.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Render the actual cube
	// Pushing camera matrix for cube set above
	glPushMatrix();

	glTranslatef(translateCubeX, translateCubeY, translateCubeZ);
	glScalef(cubeScale, cubeScale, cubeScale);
	glRotatef(angleCube, 1.0f, 1.0f, 1.0f);

	drawLitCube();
	glPopMatrix();

	// Define u=your floor as a stencil
	// Since we our defining are floor as stencil and we are not rendering it everytime
	glDisable(GL_DEPTH_TEST);

	// Mask out all the four colour components as we are not rendering for sometime
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	
	// Now enable stencil test
	glEnable(GL_STENCIL_TEST);
	
	// Alway pass the stencil test
	glStencilFunc(GL_ALWAYS, 1, 1);

	// what should i do when stencil test passes
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	
	// This call will not render the floor
	drawFloor();

	// Render the Reflected cube
	glEnable(GL_DEPTH_TEST); // start rendering again

	// Unmask all the four colors
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	// draw where only 1 is stored
	glStencilFunc(GL_EQUAL, 1, 1);

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// Pushing camera matrix for cube set above
	glPushMatrix();

	glScalef(1.0f, -1.0f, 1.0f);
	glTranslatef(translateCubeX, translateCubeY, translateCubeZ);
	glScalef(cubeScale, cubeScale, cubeScale);
	glRotatef(angleCube, 1.0f, 1.0f, 1.0f);

	drawLitCube();
	glPopMatrix();

	glDisable(GL_STENCIL_TEST);

	// Render the floor
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
	drawFloor();
	glDisable(GL_BLEND);

	// Swap the buffers
	SwapBuffers(ghdc);
}

void drawLitCube(void)
{
	// code
	glEnable(GL_LIGHTING);
	// trangle drawing code
	glBegin(GL_QUADS);

	// front face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Right Face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);  //top
	glVertex3f(1.0f, 1.0f, 1.0f);   //left top
	glVertex3f(1.0f, -1.0f, 1.0f);  //left bottom
	glVertex3f(1.0f, -1.0f, -1.0f);  //Right bottom

	// Back face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	// Left Face 
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// top face 
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom face 
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glEnd();

	glDisable(GL_LIGHTING);
}

void drawFloor(void)
{
	// code
	
	// Pushing the camera matrix set in display()
	glPushMatrix();

	// Translate Cube Backwards by Z (-ve)
	glTranslatef(0.0f, -1.0f, -2.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glScalef(5.0f, 5.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, texture_marble);

	// Cube drawing code
	glBegin(GL_QUADS);

	// Cube
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
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

	if (texture_marble)
	{
		glDeleteTextures(1, &texture_marble);
		texture_marble = 0;
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
