/****************************************************************
*	BinaryClockSS.h												*
*																*
*	Version:	0.7												*
*	Author: 	David Enderson									*
*	E-mail:		neelix@festing.org								*
*	Webpage:	http://neelix.festing.org						*
*	this source code is released under the GPL					*
*---------------------------------------------------------------*
* see the comments in BinaryClockSS.cpp							*
****************************************************************/
#include <ScreenSaver.h>
#include <time.h>
#include <math.h>
#include <Bitmap.h>
#include "SetupView.h"

class BinaryClock : public BScreenSaver
{
public:
				BinaryClock(BMessage *message, image_id id);
	void		StartConfig(BView *view);
	status_t	StartSaver(BView *v, bool preview);
	status_t 	SaveState(BMessage *into) const;
	void		Draw(BView *v, int32 frame);
	SetupView *configview;

	int width, height;			// the width & height of the screen
	BPoint drawpoint[24];		// the screen coordinates where the clock's dots
								// are drawn.

	BBitmap *CurrOn;			// contains the current "on" image 
	BBitmap *CurrOff;			// contains the current "off" image.
	
	int32 mcolor;				// integer representing the color currently selected
								// 1 = Cyan, 2 = Purple, 3 = Red
	int32 oldmcolor;			// contains the last color chosen. Used to see if the
								// user changed the color, and if so to restart the saver

	int oldsec;					// the last second to be displayed
								// used so all the redraw functions aren't
								// redrawn every time the loop is run
};
