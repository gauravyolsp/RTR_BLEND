// standard header files
#include<stdio.h>     // for printf()
#include<stdlib.h>    // for exit()
#include<memory.h>    // for memset()

// X-lib header files
#include<X11/Xlib.h>
#include<X11/Xutil.h>    // 

// MACROS
#define WIN_WIDTH     800
#define WIN_HEIGHT    600

// global variables
Display *gpDisplay = NULL;        
XVisualInfo visualInfo;
Window window;
Colormap colormap;

int main(void)
{
    // function declaration
    void uninitilize(void);

    // variable declarations
    int defaultScreen;
    int defaultDepth;
    Status status;
    XSetWindowAttributes windowAttributes;
    Atom windowManagerDeleteAtom;
    XEvent event;

    //code

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

    memset((void*)&visualInfo,0,sizeof(XVisualInfo));

    // getvisual info
    status = XMatchVisualInfo(gpDisplay,defaultScreen,defaultDepth,TrueColor,&visualInfo);
    if(status == 0)
    {
        printf("xMatchVisualInfo() failed \n");
        uninitilize();
        exit(EXIT_FAILURE);
    }

    // set window attributes
    memset((void*)&windowAttributes,0,sizeof(XSetWindowAttributes));

    windowAttributes.border_pixel = 0;
    windowAttributes.background_pixmap = 0;
    windowAttributes.background_pixel = XBlackPixel(gpDisplay,visualInfo.screen);   // we can use defaultSreen as wll for 2 'nf parameter
    windowAttributes.colormap = XCreateColormap(gpDisplay,
                                                XRootWindow(gpDisplay,visualInfo.screen),
                                                visualInfo.visual,
                                                AllocNone);
   colormap = windowAttributes.colormap;

   // create window
   window = XCreateWindow(gpDisplay,
                          XRootWindow(gpDisplay,visualInfo.screen),
                          0,
                          0,
                          WIN_WIDTH,
                          WIN_HEIGHT,
                          0,
                          visualInfo.depth,
                          InputOutput,
                          visualInfo.visual,
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

   // message loop
   while(1)
   {
       XNextEvent(gpDisplay,&event);
       switch (event.type)
       {
       case 33:
               uninitilize();
               exit(EXIT_SUCCESS);
               break;
       default:
        break;
       }
   }

   uninitilize();

   return 0;
}

void uninitilize(void)
{
    //code
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
}


