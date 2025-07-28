// Win32 Headers files
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

// OpenGl related header files
#include <GL/glew.h> // This header file must be included before "gl/GL.h"
#include <gl/GL.h>

// Matrix and transformation related maths
#include "vmath.h"
using namespace vmath;

// User defined header file
#include "OGL.h"

// OpenGl related libraries
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

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

// shader related global variables
GLuint shaderProgramObject = 0;
enum
{
	AMC_ATTRIBUTE_POSIITON = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_TEXCOORD,
};

GLuint vao_cube = 0;
GLuint vbo_texcoord_cube = 0;
GLuint vbo_position_cube = 0;

GLuint mvpMatrixUniform;
mat4 perspectiveProjectionMatrix;

GLfloat angleCube = 0.0f;

// Variables related to TEXTURE
GLuint texture_kundali;
GLuint textureSamplerUniform;

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
	int cxScreen;
	int cyScreen;

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
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	cxScreen = GetSystemMetrics(SM_CXSCREEN);		// width
	cyScreen = GetSystemMetrics(SM_CYSCREEN);		// Height
	cxScreen = (cxScreen / 2) - (WIN_WIDTH / 2);
	cyScreen = (cyScreen / 2) - (WIN_HEIGHT / 2);

	// Registration of Window Class
	RegisterClassEx(&wndclass);

	// Create Window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, 
		szAppName,
		TEXT("Gaurav Kumar"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		cxScreen,
		cyScreen,
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
	void uninitialize(void);
	BOOL loadGLTexture(GLuint*, TCHAR[]);

	// variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;
	GLenum glewResult;

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

	// Initailize GLEW
	glewResult = glewInit();

	if (glewResult != GLEW_OK)
	{
		fprintf(gpFile, "glewInit() failed !!!\n");
		return(-6);
	}

	printGLInfo();

	// VERTEX SHADER
	// Step 1 : Write shader source code
	const GLchar* vertexShaderSourceCode =
		"#version 460 core\n" \
		"in vec4 aPosition;\n" \
		"uniform mat4 uMVPMatrix;\n" \
		"in vec2 aTexCoord;\n" \
		"out vec2 out_texcoord;\n" \
		"void main(void)\n" \
		"{\n" \
		"gl_Position = uMVPMatrix * aPosition;\n" \
		"out_texcoord = aTexCoord;\n" \
		"}\n";

	// Step 2 : Create the shader object
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// Step 3 : Give the shader source code to shader object 
	glShaderSource(vertexShaderObject, 1, (const GLchar **)& vertexShaderSourceCode, NULL);
	// 2nd parameter tells us number of shader i.e we can give array of multiple shader
	// 4th parameter tells length of every shader we are giving(if we are giving array)
	
	// Step 4 : Compile the shader by giving shader object
	glCompileShader(vertexShaderObject);

	// Step 5 : Do Shader Compilation error checking
	GLint status = 0;
	GLint infoLogLength = 0;
	GLchar* szInfoLog = NULL;

	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	
	if (status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		
		// There are some errors that don't have error length like in rate tracing, so handle those errors manually
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar *)malloc(infoLogLength * sizeof(GLchar));
			if (szInfoLog != NULL)
			{
				// 3rd paramter tells actual returned length.
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, szInfoLog);

				fprintf(gpFile, "Vertex shader compilation log : %s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
			}
		}

		uninitialize();
	}

	// FRAGMENT SHADER
	const GLchar* fragmentShaderSourceCode =
		"#version 460 core\n" \
		"in vec2 out_texcoord;\n" \
		"uniform sampler2D uTextureSampler;\n" \
		"out vec4 FragColor;\n" \
		"void main(void)\n" \
		"{\n" \
		"FragColor = texture(uTextureSampler, out_texcoord);\n" \
		"}\n";

	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);

	glCompileShader(fragmentShaderObject);
	
	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;
	
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar));
			if (szInfoLog != NULL)
			{
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, szInfoLog);

				fprintf(gpFile, "Fragment shader compilation log : %s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
			}
		}

		uninitialize();
	}

	// CREATE, ATTACH, LINK SHADER PROGRAM OBJECT
	shaderProgramObject = glCreateProgram();
	
	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	// Bind shader attribute at a certain index in shader to same index in host program (CPU)
	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSIITON, "aPosition");

	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");

	glLinkProgram(shaderProgramObject);

	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar));
			if (szInfoLog != NULL)
			{
				glGetProgramInfoLog(shaderProgramObject, infoLogLength, NULL, szInfoLog);

				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);

				free(szInfoLog);
				szInfoLog = NULL;
			}
		}

		uninitialize();
	}

	// Get The required uniform location from the shader
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");
	textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "uTextureSampler");

	// Provide vertex position, color, normal, textcord etc
	const GLfloat cube_position[] = {
						// front
						1.0f,  1.0f,  1.0f, // top-right of front
						-1.0f,  1.0f,  1.0f, // top-left of front
						-1.0f, -1.0f,  1.0f, // bottom-left of front
						 1.0f, -1.0f,  1.0f, // bottom-right of front

						// right
						1.0f,  1.0f, -1.0f, // top-right of right
						1.0f,  1.0f,  1.0f, // top-left of right
						1.0f, -1.0f,  1.0f, // bottom-left of right
						1.0f, -1.0f, -1.0f, // bottom-right of right

						// back
						1.0f,  1.0f, -1.0f, // top-right of back
						-1.0f,  1.0f, -1.0f, // top-left of back
						-1.0f, -1.0f, -1.0f, // bottom-left of back
						1.0f, -1.0f, -1.0f, // bottom-right of back

						// left
						-1.0f,  1.0f,  1.0f, // top-right of left
						-1.0f,  1.0f, -1.0f, // top-left of left
						-1.0f, -1.0f, -1.0f, // bottom-left of left
						-1.0f, -1.0f,  1.0f, // bottom-right of left

						// top
						1.0f,  1.0f, -1.0f, // top-right of top
						-1.0f,  1.0f, -1.0f, // top-left of top
						-1.0f,  1.0f,  1.0f, // bottom-left of top
						1.0f,  1.0f,  1.0f, // bottom-right of top

						// bottom
						1.0f, -1.0f,  1.0f, // top-right of bottom
						-1.0f, -1.0f,  1.0f, // top-left of bottom
						-1.0f, -1.0f, -1.0f, // bottom-left of bottom
						1.0f, -1.0f, -1.0f, // bottom-right of bottom
						};

	const GLfloat cube_texcoord[] = {
						// front
						1.0f, 1.0f, // top-right of front
						0.0f, 1.0f, // top-left of front
						0.0f, 0.0f, // bottom-left of front
						1.0f, 0.0f, // bottom-right of front

						// right
						1.0f, 1.0f, // top-right of right
						0.0f, 1.0f, // top-left of right
						0.0f, 0.0f, // bottom-left of right
						1.0f, 0.0f, // bottom-right of right

						// back
						1.0f, 1.0f, // top-right of back
						0.0f, 1.0f, // top-left of back
						0.0f, 0.0f, // bottom-left of back
						1.0f, 0.0f, // bottom-right of back

						// left
						1.0f, 1.0f, // top-right of left
						0.0f, 1.0f, // top-left of left
						0.0f, 0.0f, // bottom-left of left
						1.0f, 0.0f, // bottom-right of left

						// top
						1.0f, 1.0f, // top-right of top
						0.0f, 1.0f, // top-left of top
						0.0f, 0.0f, // bottom-left of top
						1.0f, 0.0f, // bottom-right of top

						// bottom
						1.0f, 1.0f, // top-right of bottom
						0.0f, 1.0f, // top-left of bottom
						0.0f, 0.0f, // bottom-left of bottom
						1.0f, 0.0f, // bottom-right of bottom
						};

	// VERTEX ARRAY OBJECT FOR ARRAY OF VERTEX ATTRIBUTE
	// CUBE
	glGenVertexArrays(1, &vao_cube);
	glBindVertexArray(vao_cube);

	// POSITION
	// Step 1
	glGenBuffers(1, &vbo_position_cube);
	// Step 2
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position_cube);
	// Step 3
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_position), cube_position, GL_STATIC_DRAW);
	// Step 4
	glVertexAttribPointer(AMC_ATTRIBUTE_POSIITON, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// Step 5
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSIITON);
	// Step 6
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// TexCoord
	// Step 1
	glGenBuffers(1, &vbo_texcoord_cube);
	// Step 2
	glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoord_cube);
	// Step 3
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texcoord), cube_texcoord, GL_STATIC_DRAW);
	// Step 4
	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	// Step 5
	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);
	// Step 6
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Depth related code
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);

	// From here onwards OpenGL code starts
	// Tell OpenGL to choose the color to clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Load Textures
	if (loadGLTexture(&texture_kundali, MAKEINTRESOURCE(IDBITMAP_KUNDALI)) == FALSE)
	{
		fprintf(gpFile, "Load Texture Failure for Kundali.bmp");
		return(-7);
	}

	perspectiveProjectionMatrix = mat4::identity();

	// Warmup resize
	resize(WIN_WIDTH, WIN_HEIGHT);
	return(0);
}

void printGLInfo(void)
{
	// VARIABLE DECLARATIONS
	GLint numExtensions, i;

	// code
	// print OpenGL Information
	fprintf(gpFile, "OPENGL INFORMATION\n");
	fprintf(gpFile, "************************************************\n");
	fprintf(gpFile, "OpenGL Vendor   : %s\n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer : %s\n", glGetString(GL_RENDERER));
	fprintf(gpFile, "OpenGL Version  : %s\n", glGetString(GL_VERSION));
	fprintf(gpFile, "GLSL Version  : %s\n"	, glGetString(GL_SHADING_LANGUAGE_VERSION));
	fprintf(gpFile, "************************************************\n");

	// Get number of extensions
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	fprintf(gpFile, "Number of Extensions = %d\n", numExtensions);
	// print OpenGL extensions
	for (i = 0; i < numExtensions; i++)
	{
		fprintf(gpFile, "%s\n", glGetStringi(GL_EXTENSIONS, i));
	}
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
		//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

		glGenerateMipmap(GL_TEXTURE_2D);
		// Un-Bind 0 denotes unbind
		glBindTexture(GL_TEXTURE_2D, 0);

		DeleteObject(hBitMap);
		hBitMap = NULL;
	}

	return bResult;
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

	perspectiveProjectionMatrix = vmath::perspective(45.0f, // FOV-Y 
		(GLfloat)width / (GLfloat)height, // Aspect Ratio 
		0.1f, // Near 
		100.0f); // Far

}

void display(void)
{
	// code
	// Clear OpenGl Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// USE SHADER PROGRAM OBJECT
	glUseProgram(shaderProgramObject);

	// CUBE

	// Transformation
	mat4 modelViewMatrix = mat4::identity(); // Equal to glLoadIdentity in Display for model view matrix

	mat4 modelViewProjectionMatrix = mat4::identity();

	mat4 translationMatrix = mat4::identity();
	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

	mat4 scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	mat4 rotationMatrix = mat4::identity();

	mat4 rotationMatrixX = mat4::identity();
	rotationMatrixX = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);
	mat4 rotationMatrixY = mat4::identity();
	rotationMatrixY = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);
	mat4 rotationMatrixZ = mat4::identity();
	rotationMatrixZ = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);
	
	rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;
	
	modelViewMatrix = translationMatrix * scaleMatrix * rotationMatrix; // order is very important

	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix; // Order is important

	// Send this matrix to the shader in uniform
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	// For Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_kundali);
	glUniform1i(textureSamplerUniform, 0);

	// Bind with VAO_CUBE
	glBindVertexArray(vao_cube);

	// Draw the vertex arrays
	// shader start running here
	// 2nd parameters tells index of array from where to start 0 se 4, 4 se 4 , 8 se 4 and so on
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	// Unused shader program object
	glUseProgram(0);

	// Swap the Buffers
	SwapBuffers(ghdc);
}

void update(void)
{
	// code
	angleCube = angleCube - 0.05f;

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

	if (texture_kundali)
	{
		glDeleteTextures(1, &texture_kundali);
		texture_kundali = 0;
	}

	// CUBE
	// Free vbo of texture
	if (vbo_texcoord_cube)
	{
		glDeleteBuffers(1, &vbo_texcoord_cube);
		vbo_texcoord_cube = 0;
	}

	// Free vbo of position
	if (vbo_position_cube)
	{
		glDeleteBuffers(1, &vbo_position_cube);
		vbo_position_cube = 0;
	}

	// Free VAO
	if (vao_cube)
	{
		glDeleteVertexArrays(1, &vao_cube);
		vao_cube = 0;
	}

	// DETACH, DELETE SHADER OBJECT AND DELETE SHADER PROGRAM OBJECT
	if (shaderProgramObject)
	{
		glUseProgram(shaderProgramObject);
		GLint numShaders = 0;

		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numShaders);

		if (numShaders > 0)
		{
			GLuint* pShaders = (GLuint *) malloc(numShaders * sizeof(GLuint));

			if (NULL != pShaders)
			{
				glGetAttachedShaders(shaderProgramObject, numShaders, NULL, pShaders);

				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachShader(shaderProgramObject, pShaders[i]);
					glDeleteShader(pShaders[i]);
					pShaders[i] = 0;
				}
			}

			free(pShaders);
			pShaders = NULL;
		}

		glUseProgram(0);
		glDeleteProgram(shaderProgramObject);
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
