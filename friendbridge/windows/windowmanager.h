#ifndef _windowmanager_h_
#define _windowmanager_h_
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <string.h>

typedef struct WindowClassEntry_s
{
	struct WindowClassEntry_s *head;
	struct WindowClassEntry_s *tail;
	struct WindowClassEntry_s *next;
	char *className;
	void *data;
} WindowClassEntry;

typedef struct WindowEntry_s
{
	struct WindowEntry_s *head;
	struct WindowEntry_s *tail;
	struct WindowEntry_s *next;
	char *windowName;
	Display *display;
	Window *window;
} WindowEntry;

WindowClassEntry *CreateWindowMatrix();

void FreeWindowMatrix( WindowClassEntry *matrix );

void RefreshWindowMatrix( Display *display, WindowClassEntry *matrix );

int WindowMatrixHasClass( char *className, WindowClassEntry *matrix );

WindowClassEntry *WindowMatrixGetClassEntry( char *className, WindowClassEntry *matrix );

int WindowMatrixAddClass( char *className, WindowClassEntry *matrix );

int WindowMatrixAddWindow( char *className, char *windowName, Display *display, Window *window, WindowClassEntry *matrix );

void FreeWindowEntry( WindowEntry *entry );

int MoveWindowToLayer( Display *display, Window window, char *layerName );

void PrintWindowInfo( Display *display, Window window );

void ControlWindows( Display *display );












































#endif
