// standard header files
#include <stdio.h>     // for printf()
#include <stdlib.h>    // for exit()
#include <memory.h>    // for memset()

// X-lib header files
#include <X11/Xlib.h> // windows.h
#include <X11/Xutil.h>    // for visualinfo and related API
#include <X11/XKBlib.h> // Keyboard related API's

// OpenGL related Header files
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

// MACROS
#define WIN_WIDTH     800
#define WIN_HEIGHT    600

// variables related with file I/O
char gszLogFileName[] = "Log.txt";
FILE *gpFile = NULL;

// global variables
Display *gpDisplay = NULL;        
XVisualInfo *visualInfo = NULL;
Window window;
Colormap colormap;

// Global variables for Full Screen
Bool bFullScreen = False;
Bool bActiveWindow = False;

// OpenGL related variables
GLXContext glxContext = NULL;

// My Global Variables
float o[3] = {1.0f, 0.6f, 0.2f};
float w[3] = {1.0f, 1.0f, 1.0f};
float g[3] = {0.0745f, 0.5333f, 0.0314f};

GLfloat g_MoveB = -1.5f;
GLfloat g_MoveHx = -1.0f;
GLfloat g_MoveHy = 1.0f;

int main(void)
{
    // function declaration
    void toggleFullscreen(void);
    int  initialize(void);
    void resize(int, int);
    void display(void);
    void update(void);
    void uninitilize(void);

    // variable declarations
    int defaultScreen;
    int defaultDepth;
    XSetWindowAttributes windowAttributes;
    Atom windowManagerDeleteAtom;
    XEvent event;
    
    // local variables for window centering
    Screen *screen = NULL;
    int screenWidth, screenHeight;

    // Key symbol
    KeySym keySym;
    char keys[52] = {0}; // All though we only need 0th index, but for convension we take array for all Alphate capital and small
    
    int framebufferAttributes[] = {GLX_DOUBLEBUFFER,
                                GLX_RGBA,
                                GLX_RED_SIZE,   8,
                                GLX_GREEN_SIZE, 8,
                                GLX_BLUE_SIZE,  8,
                                GLX_ALPHA_SIZE, 8,
                                None};
    int iResult = 0;
    // for Game Loop
    Bool bDone = False;
    
    //code

	// Create Log File
	gpFile = fopen(gszLogFileName, "w");

	if(gpFile == NULL)
	{
		printf("LOG FILE CREATION FAILED !!!");
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Program started successfully\n");
	}

    // open connection with x server
    gpDisplay = XOpenDisplay(NULL);

    if(gpDisplay == NULL)
    {
        printf("XOpenDisplay failed to connect with X-Server \n");
        uninitilize();
        exit(EXIT_FAILURE);
    }

    // create default screen object
    defaultScreen = XDefaultScreen(gpDisplay);

    // get default depth
    defaultDepth = XDefaultDepth(gpDisplay,defaultScreen);

    visualInfo = glXChooseVisual(gpDisplay, defaultScreen, framebufferAttributes);
    if(NULL == visualInfo)
    {
        printf("glXChooseVisual() failed \n");
        uninitilize();
        exit(EXIT_FAILURE);
    }

    // set window attributes
    memset((void*)&windowAttributes,0,sizeof(XSetWindowAttributes));

    windowAttributes.border_pixel = 0;
    windowAttributes.background_pixmap = 0;
    windowAttributes.background_pixel = XBlackPixel(gpDisplay,visualInfo->screen);   // we can use defaultSreen as wll for 2 'nf parameter
    windowAttributes.colormap = XCreateColormap(gpDisplay,
                                                XRootWindow(gpDisplay,visualInfo->screen),
                                                visualInfo->visual,
                                                AllocNone);
   colormap = windowAttributes.colormap;

   windowAttributes.event_mask = KeyPressMask | ButtonPressMask | FocusChangeMask | StructureNotifyMask | ExposureMask;

   // create window
   window = XCreateWindow(gpDisplay,
                          XRootWindow(gpDisplay, visualInfo->screen),
                          0,
                          0,
                          WIN_WIDTH,
                          WIN_HEIGHT,
                          0,
                          visualInfo->depth,
                          InputOutput,
                          visualInfo->visual,
                          CWBorderPixel | CWBackPixel| CWEventMask | CWColormap,
                          &windowAttributes);

    if(!window)
    {
        printf("x-Create window failed \n");
        uninitilize();
        exit(EXIT_FAILURE);
    }

    // create atom for window manager to distroy window
    windowManagerDeleteAtom = XInternAtom(gpDisplay,"WM_DELETE_WINDOW",True);
    
    XSetWMProtocols(gpDisplay,window,&windowManagerDeleteAtom,1);

    // set window tittle
    XStoreName(gpDisplay,window, " Gaurav : XWindow");

    // map the window to show
    XMapWindow(gpDisplay,window);

    // Centering of window
    screen = XScreenOfDisplay(gpDisplay, visualInfo->screen);
    screenWidth = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window, (screenWidth/2 - WIN_WIDTH/2), (screenHeight/2 - WIN_HEIGHT/2));

    // initialize
    iResult = initialize();
    if(iResult == -1)
	{
		// fprintf(gpFile, "Initailize() Failed !!!\n");
        uninitilize();
        exit(EXIT_FAILURE);
	}
	else
	{
		// fprintf(gpFile, "Initailize() Completed Successfully");
	}

    // Game Loop
    while(bDone == False)
    {
        while(XPending(gpDisplay))
        {
            XNextEvent(gpDisplay,&event);
            switch (event.type)
            {
                case MapNotify:
                break;
                case FocusIn:
                    bActiveWindow = True;
                break;
                case FocusOut:
                    bActiveWindow = False;
                break;
                case ConfigureNotify:
                    resize(event.xconfigure.width, event.xconfigure.height);
                break;
                case KeyPress:
                {
                    // For Escape key (having key code)
                    keySym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);

                    switch(keySym)
                    {
                        case XK_Escape:
                            bDone = True;
                        break;
                        default:
                        break;
                    }

                    // For Alphabetic key press
                    XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL);

                    switch(keys[0])
                    {
                        case 'F':
                        case 'f':
                            if(bFullScreen == False)
                            {
                                toggleFullscreen();
                                bFullScreen = True;
                            }
                            else
                            {
                                toggleFullscreen();
                                bFullScreen = False;
                            }
                        break;
                        default:
                        break;
                    }
                }break;
                case ButtonPress:
                break;
                case Expose: // WM_PAINT
                break;
                case 33:
                    bDone = True;
                break;
                default:
                break;
            }
        }

        // Rendering
        if(bActiveWindow == True)
        {
            display();

            update();
        }
    }

    uninitilize();
    return 0;
}

void toggleFullscreen(void)
{
    // code
    Atom windowMangerNormalStateAtom = XInternAtom(gpDisplay, "_NET_WM_STATE", False);
    Atom windowManagerFullscreenStateAtom = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);
    XEvent event;
    memset((void *)&event, 0x00, sizeof(XEvent));

    event.type = ClientMessage;
    event.xclient.window = window;
    event.xclient.message_type = windowMangerNormalStateAtom;
    event.xclient.format = 32;
    event.xclient.data.l[0] = bFullScreen ? 0 : 1;
    event.xclient.data.l[1] = windowManagerFullscreenStateAtom;

    // send above event to X Server
    XSendEvent(gpDisplay, 
            XRootWindow(gpDisplay, visualInfo->screen),
            False, // child ko full screen krna hai ya nahi?
            SubstructureNotifyMask,
            &event);
}

int initialize(void)
{
    // Function declarations
	void printGLInfo(void);
	void resize(int, int);

    // code
    glxContext = glXCreateContext(gpDisplay, visualInfo, NULL, True);

    if(glxContext == NULL)
    {
        printf("glXCreateContext() fails\n");
        return -1;
    }

    glXMakeCurrent(gpDisplay, window, glxContext);

    printGLInfo();

    // clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Warmup resize
	resize(WIN_WIDTH, WIN_HEIGHT);
    return 0;
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
    if(height <= 0)
    {
        height = 1;
    }

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set Matrix to model view mode
	glMatrixMode(GL_MODELVIEW);

	// Set to identity matirx
	glLoadIdentity();

    // Translate Triangle Backwards by Z (-ve)
	// glTranslatef(0.1f, -0.3f, -2.0f);

	// BHARAT
	glPushMatrix();
	B();
	glPopMatrix();
	glPushMatrix();
	H();
	glPopMatrix();
	// A();
	// R();
	// A1();
	// T();

    glXSwapBuffers(gpDisplay, window);
}

void update(void)
{
    // code
}

void uninitilize(void)
{
    //code

    GLXContext currentContext = glXGetCurrentContext();

    if(currentContext && currentContext == glxContext)
    {
        glXMakeCurrent(gpDisplay, 0, 0);
    }
    if(glxContext)
    {
        glXDestroyContext(gpDisplay, glxContext);
        glxContext = NULL;
    }

    if(visualInfo)
    {
        free(visualInfo);
        visualInfo = NULL;
    }

    if(window)
    {
        XDestroyWindow(gpDisplay,window);
    }

    if(colormap)
    {
        XFreeColormap(gpDisplay,colormap);
    }

    if(gpDisplay)
    {
        XCloseDisplay(gpDisplay);
        gpDisplay = NULL;
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
	// glLoadIdentity();
	glTranslatef(g_MoveB, -0.3f, -5.0f);
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

	if(g_MoveB < 0.05f)
		g_MoveB = g_MoveB + 0.0001f;
}

// H
void H(void)
{
	// glLoadIdentity();
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
	//glEnd();

	if(g_MoveHx < 0.0001f)
		g_MoveHx = g_MoveHx + 0.1f;
	if(g_MoveHy < 0.0001f)
		g_MoveHy = g_MoveHy - 0.1f;
}

// A
void A(void)
{
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
}

// R
void R(void)
{

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
		/*glColor3f(o[0], o[1], o[2]);
		glVertex3f(0.2f, 0.6f, 0.0f);
		glVertex3f(0.15f, 0.55f, 0.0f);
		glVertex3f(0.15f, 0.3f, 0.0f);
		glVertex3f(0.2f, 0.3f, 0.0f);*/

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
}

// A1
void A1(void)
{
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
}

// T
void T(void)
{
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
}
