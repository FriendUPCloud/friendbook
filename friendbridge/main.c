/*©mit**************************************************************************
*                                                                              *
* This file is part of FRIEND UNIFYING PLATFORM.                               *
* Copyright (c) Friend Software Labs AS. All rights reserved.                  *
*                                                                              *
* Licensed under the Source EULA. Please refer to the copy of the MIT License, *
* found in the file LOCENSE.                                                   *
*                                                                              *
*****************************************************************************©*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include "windows/windowmanager.h"
#include "communication/communication.h"

void HandleWindowCreated( Display *display, Window window )
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
	Window id;
	do
	{
		id = FindWindowByName( "Sign into your account" );
		usleep( 50000 );
	}
	while( id == 0 );
	
	printf( "Moving window %ld to below\n", ( long int )id );
	MoveWindowToLayer( id, "below" );
	
	/*// Collect all windows in the window matrix
	RefreshWindowMatrix( matrix, display );

	XEvent ev;
    while( 1 )
    {
        XNextEvent( display, &ev );
        if( ev.type == CreateNotify )
        {
            HandleWindowCreated( display, ev.xcreatewindow.window );
        }
    }*/

    XCloseDisplay( display );
    
    // Clear memory used by window matrix
    FreeWindowMatrix( matrix );
    return 0;
}
