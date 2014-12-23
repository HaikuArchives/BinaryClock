/****************************************************************
*				BinaryClockWindow.cpp							*
*				Version 2.0										*
*				by David Enderson (david@enderson.net)			*
*---------------------------------------------------------------*
* This code is Copyright 2000 under the GPL.					*
* See the Readme file included in this directory for more info.	*
****************************************************************/
#include <string.h>
#include <sys/stat.h>
#include <Debug.h>
#include <FindDirectory.h>
#include <Path.h>

#include "BinaryClockWindow.h"

TBCWindow::TBCWindow(BRect windowRect, const char* t)
		:BWindow(windowRect, t,
			B_FLOATING_WINDOW_LOOK,
			B_NORMAL_WINDOW_FEEL,
			B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_AVOID_FRONT,
			B_CURRENT_WORKSPACE)
{
	// Hello, and thanks for reading my source.
	// Functions can't be TOTALLY empty, can they, so 
	//   I thought I'd say hello to you.  :-)

	// OH!  I need something here anyway.  Surprise surprise.
	SetPulseRate(1000000);
}

bool TBCWindow::QuitRequested( void )
{
	int			ref;
	BPoint		lefttop;
	int			face;
	bool		twentyfr;
	BPath		path;
	BRect		thebounds;
	
	if (find_directory (B_USER_SETTINGS_DIRECTORY, &path, true) == B_OK) {
		path.Append("BinaryClock/settings");
		ref = creat(path.Path(), 0777);
		if (ref >= 0) {
			lefttop = Frame().LeftTop();
			write(ref, (char *)&lefttop, sizeof(BPoint));
			face = TheView->numcolor;
			write(ref, (char *)&face, sizeof(int));
			thebounds = Frame();
			write(ref, (char *)&thebounds, sizeof(BRect));
			twentyfr = TheView->twentyfour;
			write(ref, (char *)&twentyfr, sizeof(bool));
			close(ref);
		}
	}
	be_app->PostMessage(B_QUIT_REQUESTED);
	return(TRUE);
}
