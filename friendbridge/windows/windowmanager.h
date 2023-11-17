#ifndef _windowmanager_h_
#define _windowmanager_h_
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <string.h>

typedef struct WindowClassEntry
{
	void *head;
	void *tail;
	void *next;
	char *className;
	void *data;
};

typedef struct WindowEntry
{
	void *head;
	void *tail;
	void *next;
	Display *display;
	Window *window;
};

WindowClassEntry *WindowMatrix;

void FreeClassHierarchy( WindowClassEntry *matrix );

void RefreshClassHierarchy( Display *display );

int MoveWindowToLayer( Display *display, Window window, char *layerName );

void PrintWindowInfo( Display *display, Window window );

void ControlWindows( Display *display );












































#endif
