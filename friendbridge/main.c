#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <X11/Xlib.h>

void handleWindowCreated( Window window, Display *display )
{
    // Retrieve and print the window title
    char *windowTitle = NULL;
    XFetchName( display, window, &windowTitle );
    if( windowTitle != NULL )
    {
       	printf( "New window created with ID: %lu, Title: %s\n", window, windowTitle );
        XFree( windowTitle );  // Free the memory allocated by XFetchName
    } 
    else 
    {
        printf( "New window created with ID: %lu\n", window );
    }

}

int main()
{
    Display *display = XOpenDisplay( NULL );
    if(display == NULL)
    {
        fprintf(stderr, "Unable to open display.\n");
        return 1;
    }

    Window root = DefaultRootWindow( display );
    Window parent, *children;
    unsigned int numChildren;

    if( XQueryTree(display, root, &root, &parent, &children, &numChildren ) ) 
    {
        for( unsigned int i = 0; i < numChildren; i++ )
        {
            handleWindowCreated( children[ i ], display );
        }
        XFree( children );
    } 
    else 
    {
        fprintf( stderr, "Unable to query window tree.\n" );
    }

    XCloseDisplay( display );

    return 0;
}


