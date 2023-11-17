#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>

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

// Find and handle the workspace window
void handleWorkspaceWindow()
{
	
}

int main()
{
    Display *display = XOpenDisplay( NULL );
    if( display == NULL )
    {
        fprintf( stderr, "Unable to open display.\n" );
        return 1;
    }

    XSelectInput( display, DefaultRootWindow( display ), SubstructureNotifyMask );


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
    return 0;
}
