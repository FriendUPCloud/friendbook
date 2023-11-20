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

int MoveWindowToLayer( Display *display, Window window, char *layerName )
{
	if( layerName == NULL ) return 0;
	if( strcmp( layerName, "below" ) == 0 )
	{
		Window root = DefaultRootWindow( display );

		XWindowChanges wc;
		wc.sibling = root;
		wc.stack_mode = Below;

		XConfigureWindow(display, window, CWSibling | CWStackMode, &wc);
	}
	return 0;
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
void RefreshWindowMatrix( Display *display, WindowClassEntry *matrix )
{
	FreeWindowMatrix( matrix );
	matrix = CreateWindowMatrix();
	
	Window root = DefaultRootWindow( display );
	Window parent, *children;
	unsigned int nchildren = 0;
	
	if( XQueryTree( display, root, &root, &parent, &children, &nchildren ) )
	{
        for( unsigned int i = 0; i < nchildren; i++ )
        {
        	XClassHint classHint;
			char *className, *windowName;
			Window window = children[ nchildren ];
			if( XGetClassHint( display, &window, &classHint ) )
			{
            	className = classHint.res_name ? classHint.res_name : "Unknown";
				windowName = classHint.res_class ? classHint.res_class : "Unknown";
				printf( "Class: %s\n", className );
				printf( "Instance: %s\n", windowName );
				
				// Check if window already has class
				if( !WindowMatrixHasClass( className, matrix ) )
				{
					// If not, then create it
					WindowMatrixAddClass( className, matrix );
				}
				
				// Add the window to the class of windows in the matrix
				WindowMatrixAddWindow( className, &window, matrix );
								
				XFree( classHint.res_name );
				XFree( classHint.res_class );
        	}
        }
        XFree( children );
    }
}


int WindowMatrixHasClass( char *className, WindowClassEntry *matrix )
{
	WindowClassEntry *pos = matrix;
	do
	{
		if( strcmp( pos->className, className ) == 0 )
			return 1;
	}
	while( ( pos = pos->next ) != NULL );
	return 0;
}

int WindowMatrixAddClass( char *className, WindowClassEntry *matrix )
{
	WindowClassEntry *new = NULL;
	// First entry, set new to matrix
	if( matrix->head == matrix->tail && matrix->head == matrix )
	{
		new = matrix;
	}
	else
	{
		// Make sure it does not fail
		if( ( new = calloc( 1, sizeof( WindowClassEntry ) ) ) == NULL )
		{
			return 0;
		}
	}
	
	// Copy class name, make sure it does not fail
	new->className = calloc( 1, strlen( className ) + 1 );
	if( new->className == NULL )
	{
		if( new != matrix )
			free( new );
		return 0;
	}
	snprintf( new->className, strlen( className ), "%s", className );
	
	matrix->tail->next = new; // Attach new to current tail's next
	matrix->tail = ( void *)new;       // Attach new to current tail
	new->head = matrix->head; // New head 
	new->tail = new;          // New tail is self
	new->next = NULL;         // There's no next
	new->data = NULL;         // There's no data
	return 1;
}

void WindowMatrixAddWindow( char *className, Window *window, WindowClassEntry *matrix )
{
	return;
}


