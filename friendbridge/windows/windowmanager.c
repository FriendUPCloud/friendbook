#include "windowmanager.h"

int moveWindowToLayer( Display *display, Window window, char *layerName )
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

