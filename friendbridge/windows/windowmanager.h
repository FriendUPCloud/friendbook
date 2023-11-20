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
	char *windowTitle;
	Display *display;
	Window *window;
} WindowEntry;

WindowClassEntry *CreateWindowMatrix();

void FreeWindowMatrix( WindowClassEntry *matrix );

void RefreshWindowMatrix( WindowClassEntry *matrix, Display *display );

int WindowMatrixHasClass( WindowClassEntry *matrix, char *className );

WindowClassEntry *WindowMatrixGetClassEntry( WindowClassEntry *matrix, char *className );

int WindowMatrixAddClass( WindowClassEntry *matrix, char *className );

int WindowMatrixAddWindow( WindowClassEntry *matrix, char *className, char *windowName, char *windowTitle, Display *display, Window *window );

void FreeWindowEntry( WindowEntry *entry );

int MoveWindowToLayer( Display *display, Window window, char *layerName );

void PrintWindowInfo( Display *display, Window window );

void ControlWindows( Display *display );












































#endif
