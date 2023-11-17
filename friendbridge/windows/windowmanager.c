#include "windowmanager.h"

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

