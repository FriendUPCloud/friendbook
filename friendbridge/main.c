#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include "windows/windowmanager.h"
#include "communication/communication.h"

void handleWindowCreated( Display *display, Window window )
{
    printf( "New window created with ID: %lu\n", window );

    // Retrieve and print the window title
    char *windowTitle = NULL;
    XFetchName( display, window, &windowTitle );
    if( windowTitle != NULL )
    {
        printf( "Window title: %s\n", windowTitle );
        XFree( windowTitle );  // Free the memory allocated by XFetchName
    }
    else
    {
        printf( "Unable to retrieve window title.\n" );
    }
    // Add your custom processing or code to run when a window is created
}

int main()
{
    Display *display = XOpenDisplay( NULL );
    if( display == NULL )
    {
        fprintf( stderr, "Unable to open display.\n" );
        return 1;
    }

	// Create window matrix
	WindowClassEntry *matrix = CreateWindowMatrix();

    XSelectInput( display, DefaultRootWindow( display ), SubstructureNotifyMask );

	// Just control all windows
	ControlWindows( display );

    XEvent ev;
    while( 1 )
    {
        XNextEvent( display, &ev );
        if( ev.type == CreateNotify )
        {
            handleWindowCreated( display, ev.xcreatewindow.window );
        }
    }

    XCloseDisplay( display );
    
    // Clear memory used by window matrix
    FreeWindowMatrix( matrix );
    return 0;
}
