/*©mit**************************************************************************
*                                                                              *
* This file is part of FRIEND UNIFYING PLATFORM.                               *
* Copyright (c) Friend Software Labs AS. All rights reserved.                  *
*                                                                              *
* Licensed under the Source EULA. Please refer to the copy of the MIT License, *
* found in the file LOCENSE.                                                   *
*                                                                              *
*****************************************************************************©*/

#include "windowmanager.h"

WindowClassEntry *CreateWindowMatrix()
{
	WindowClassEntry *wm = calloc( 1, sizeof( WindowClassEntry ) );
	wm->next = NULL;
	wm->tail = wm;
	wm->head = wm;
	wm->data = NULL;
	wm->className = NULL;
	return wm;
}

/*int MoveWindowToLayer( Display *display, Window window, char *layerName )
{
	if( layerName == NULL ) return 0;
	if( strcmp( layerName, "below" ) == 0 )
	{
		Window root = DefaultRootWindow( display );

		XWindowChanges wc;
		wc.sibling = root;
		wc.stack_mode = Below;

		if( XConfigureWindow( display, window, CWSibling | CWStackMode, &wc ) == 0 )
		{
			printf( "Moved window correctly!\n" );
			return 1;
		}
	}
	printf( "Failed to move window...\n" );
	return 0;
}*/

int MoveWindowToLayer( unsigned long windowID, const char *layerName )
{
   	if( strcmp( "below", layerName ) == 0 )
	{
		char command[ 100 ];
		snprintf( command, sizeof( command ), "wmctrl -i -r 0x%lx -b add,below", windowID );

		if( system( command ) == -1 )
		{
		    perror( "Error running wmctrl" );
		    return 1; // Return an error code if the function fails
		}
	}

    return 0;
}

Window FindWindowByName( const char *windowName )
{
    // Construct the xdotool command
    char command[ 100 ];
    snprintf( command, sizeof( command ), "xdotool search --name %s", windowName );

    // Open a pipe to capture the output of the command
    FILE *pipe = popen( command, "r" );
    if( pipe == NULL )
    {
        perror( "Error opening pipe" );
        return 0;  // Return 0 to indicate an error
    }

    // Read the output of the command into a buffer
    char buffer[ 256 ];
    if( fgets( buffer, sizeof( buffer ), pipe ) != NULL )
    {
        // Remove newline characters from the output
        size_t len = strlen( buffer );
        if( len > 0 && buffer[ len - 1 ] == '\n' )
        {
            buffer[ len - 1 ] = '\0';
        }

        // Convert the string to a window ID
        Window windowID = strtoul( buffer, NULL, 0 );

        // Close the pipe
        if( pclose( pipe ) == -1 )
        {
            perror( "Error closing pipe" );
        }
        return windowID;
    }
    else
    {
        perror( "Error reading from pipe" );
        // Close the pipe and return 0 to indicate an error
        pclose( pipe );
        return 0;
    }
}


char *GetWindowTitle( Display *display, int windowId )
{

    if (display == NULL) 
    {
        fprintf( stderr, "Unable to open display.\n" );
        return NULL;
    }

    Window window = windowId;

    // Construct the xdotool command
    char command[ 100 ];
    snprintf( command, sizeof( command ), "xdotool getwindowname %lu", ( unsigned long )window );

    // Open a pipe to capture the output of the command
    FILE *pipe = popen( command, "r" );
    if( pipe == NULL )
    {
        perror( "Error opening pipe" );
        return NULL;
    }

    // Read the output of the command into a buffer
    char buffer[ 256 ];
    char *windowTitle = NULL;
    
    
    if( fgets( buffer, sizeof( buffer ), pipe ) != NULL )
    {
        // Remove newline characters from the output
        size_t len = strlen( buffer );
        if( len > 0 && buffer[len - 1] == '\n' )
        {
            buffer[ len - 1 ] = '\0';
        }

        // Now, 'buffer' contains the window title
        printf( "Window Title: %s\n", buffer );

        // If needed, you can copy the title to a dynamically allocated string
        windowTitle = strdup( buffer );
    }
    else 
    {
        perror( "Error reading from pipe" );
    }

    // Close the pipe
    if( pclose( pipe ) == -1 )
    {
        perror( "Error closing pipe" );
        if( windowTitle ) free( windowTitle );
        return NULL;
    }

    return windowTitle;
}


// Gets some info about a window, like title, id, class and instance names
void PrintWindowInfo( Display *display, Window window )
{
    XTextProperty prop;
    char **list = NULL;
    int count = 0;

    // Get window title
    int foundTitle = False;
    if( XGetTextProperty( display, window, &prop, XA_WM_NAME ) && prop.value && prop.nitems > 0 )
    {
        if( XmbTextPropertyToTextList( display, &prop, &list, &count ) >= Success && count > 0 )
        {
            printf( "Window Title: %s\n", list[ 0 ] );
            foundTitle = True;
            XFreeStringList( list );
        }
    }
    if( !foundTitle )
    {
    	printf( "Window Title: <Unknown>\n" );
    }
    

	XClassHint classHint;
    char *className, *windowName;
    if( XGetClassHint( display, window, &classHint ) )
    {
        className = classHint.res_name ? classHint.res_name : "Unknown";
        windowName = classHint.res_class ? classHint.res_class : "Unknown";
        printf( "Class: %s\n", className );
        printf( "Instance: %s\n", windowName );
        XFree( classHint.res_name );
        XFree( classHint.res_class );
    }
	
    printf( "Window ID: %lu\n", window );
    printf( "------------------------------------\n" );
}

// Control all windows
void ControlWindows( Display *display )
{
	Window root = DefaultRootWindow( display );
	Window parent, *children;
	unsigned int nchildren = 0;
	
	if( XQueryTree( display, root, &root, &parent, &children, &nchildren ) )
	{
        for( unsigned int i = 0; i < nchildren; i++ )
        {
            PrintWindowInfo( display, children[ i ] );
        }
        XFree( children );
    }
}

// Free window class matrix
void FreeWindowMatrix( WindowClassEntry *matrix )
{
	if( matrix == NULL ) return;
	WindowClassEntry *next = ( WindowClassEntry *)matrix->next;
	if( next != NULL )
	{
		FreeWindowMatrix( next );
	}
	if( matrix->className )
	{
		printf( "[FreeClassHierarchy] Freeing window class scope: %s\n", matrix->className );
	}
	else
	{
		printf( "[FreeClassHierarchy] Freeing window class scope: <unknown>\n" );
	}
	if( matrix->data != NULL )
	{
		FreeWindowEntry( ( WindowEntry *)matrix->data );
	}
	free( matrix );
}

// Free a window entry
void FreeWindowEntry( WindowEntry *window )
{
	if( window == NULL ) return;
	
	free( window );
}

// Rebuild window class matrix
void RefreshWindowMatrix( WindowClassEntry *matrix, Display *display )
{
	FreeWindowMatrix( matrix );
	matrix = CreateWindowMatrix();
	
	Window root = DefaultRootWindow( display );
	Window parent, *children;
	unsigned int nchildren = 0;
	XTextProperty prop;
	
	if( XQueryTree( display, root, &root, &parent, &children, &nchildren ) )
	{
        for( unsigned int i = 0; i < nchildren; i++ )
        {
        	XClassHint classHint;
			char *className, *windowName;
			Window window = children[ i ];
			if( XGetClassHint( display, window, &classHint ) )
			{
            	className = classHint.res_name ? classHint.res_name : "Unknown";
				windowName = classHint.res_class ? classHint.res_class : "Unknown";
				char *windowTitle = NULL;
				
				if( XGetTextProperty( display, window, &prop, XA_WM_NAME ) && prop.value && prop.nitems > 0 ) 
				{
					windowTitle = ( char *)prop.value;
				}
				
				// Check if window already has class
				if( !WindowMatrixHasClass( matrix, className ) )
				{
					// If not, then create it
					WindowMatrixAddClass( matrix, className );
				}
				
				// Add the window to the class of windows in the matrix
				WindowMatrixAddWindow( matrix, className, windowName, windowTitle, display, &window );
								
				XFree( classHint.res_name );
				XFree( classHint.res_class );
        	}
        }
        XFree( children );
    }
}


int WindowMatrixHasClass( WindowClassEntry *matrix, char *className )
{
	WindowClassEntry *pos = matrix;
	do
	{
		if( pos->className )
		{
			if( strcmp( pos->className, className ) == 0 )
			{
				return 1;
			}
		}
		pos = pos->next;
	}
	while( pos != NULL );
	return 0;
}

WindowClassEntry *WindowMatrixGetClassEntry( WindowClassEntry *matrix, char *className )
{
	WindowClassEntry *pos = matrix;
	do
	{
		if( strcmp( pos->className, className ) == 0 )
		{
			return pos;
		}
		pos = pos->next;
	}
	while( pos->next != NULL );
	return NULL;
}

int WindowMatrixAddClass( WindowClassEntry *matrix, char *className )
{
	WindowClassEntry *new = NULL;
	int first = 0;
	
	// First entry, set new to matrix
	if( matrix->className == NULL )
	{
		new = matrix;
		first = 1;
	}
	else
	{
		// Make sure it does not fail
		if( ( new = calloc( 1, sizeof( WindowClassEntry ) ) ) == NULL )
			return 0;
	}
	
	// Copy class name, make sure it does not fail
	new->className = calloc( 1, strlen( className ) + 1 );
	if( new->className == NULL )
	{
		if( new != matrix )
			free( new );
		return 0;
	}
	snprintf( new->className, strlen( className ) + 1, "%s", className );
	
	// Second + sibling
	if( first == 0 )
	{
		matrix->tail->next = new;
	}
	// First sibling
	else
	{
		matrix->next = new;
	}
	matrix->tail = new;
	
	printf( "Adding class %s\n", className );
	
	new->head = matrix;       // New head 
	new->next = NULL;         // There's no next
	new->data = NULL;         // There's no data
	return 1;
}

int WindowMatrixAddWindow( WindowClassEntry *matrix, char *className, char *windowName, char *windowTitle, Display *display, Window *window )
{
	WindowClassEntry *byClass = WindowMatrixGetClassEntry( matrix, className );
	if( byClass == NULL ) return 0;
	
	if( byClass->data == NULL )
	{
		byClass->data = calloc( 1, sizeof( WindowEntry ) );
		WindowEntry *bdata = ( WindowEntry *)byClass->data;
		bdata->head = byClass->data;
		bdata->tail = byClass->data;
		bdata->next = NULL;
		bdata->display = display;
		bdata->window = window;
		bdata->windowId = *window;
		
		// Copy window name, make sure it does not fail
		bdata->windowName = calloc( 1, strlen( windowName ) + 1 );
		snprintf( bdata->windowName, strlen( windowName ) + 1, "%s", windowName );
		
		bdata->windowTitle = calloc( 1, strlen( windowTitle ) + 1 );
		snprintf( bdata->windowTitle, strlen( windowTitle ) + 1, "%s", windowTitle );
		
		char *title = GetWindowTitle( display, *window );
		printf( " > In %s, added 1st (%ld) windowindow \"%s\" titled: %s (%s)\n", className, *window, windowName, windowTitle, title );
		free( title );
	}
	else
	{
		WindowEntry *wins = ( ( WindowEntry *)byClass->data )->tail;
		WindowEntry *new = calloc( 1, sizeof( WindowEntry ) );
		
		// Move offsets
		wins->next = new;
		wins->tail = new;
		
		// Set data on window entry
		new->tail = new;
		new->head = wins->head;
		new->next = NULL;
		new->display = display;
		new->window = window;
		new->windowId = *window;
		
		// Copy window name, make sure it does not fail
		new->windowName = calloc( 1, strlen( windowName ) + 1 );
		snprintf( new->windowName, strlen( windowName ) + 1, "%s", windowName );
		
		new->windowTitle = calloc( 1, strlen( windowTitle ) + 1 );
		snprintf( new->windowTitle, strlen( windowTitle ) + 1, "%s", windowTitle );
		
		char *title = GetWindowTitle( display, *window );
		printf( " > In %s, added (%ld) window \"%s\" titled: %s (%s)\n", className, *window, windowName, windowTitle, title );
	    free( title );
	}
	return 1;
}

// Get a window by title and class name
Window *GetWindowFromMatrix( WindowClassEntry *matrix, char *className, char *windowTitle )
{
	// Get position in classes
	WindowClassEntry *position = WindowMatrixGetClassEntry( matrix, className );
	if( position == NULL ) return ( Window *)NULL;
	return ( Window *)NULL;
}


