/****************************************************************
*				BinaryClockView.h								*
*				Version 2.0										*
*				by David Enderson (david@enderson.net)			*
*---------------------------------------------------------------*
* This code is Copyright 2000 under the GPL.					*
* See the Readme file included in this directory for more info.	*
****************************************************************/
#ifndef _APPLICATION_H
#include <Application.h>
#endif
#ifndef _WINDOW_H
#include <Window.h>
#endif
#ifndef _VIEW_H
#include <View.h>
#endif
#ifndef _BITMAP_H
#include <Bitmap.h>
#endif
#include <time.h>

#include "BinaryClockMenu.h"

#define APP_SIGNATURE "application/x-vnd.BinaryClock_2.0"
#define SM_GAP 1	// the gap between each graphic and the outside edge - in pixels
#define LG_GAP .5	// the width of the center column - a ratio to the width of the graphics

class _EXPORT TBCView : public BView {
public:
	// standard constructor for when the view is on the window
					TBCView(BRect viewRect, char *name, int face, bool twentyfr);
	// standard destructor
	virtual			~TBCView();
	// revives the app and puts it together as a replicant 
					TBCView(BMessage *data);
	// receives the revive call and calls the message only constructor
	static	BArchivable	*Instantiate(BMessage *data);
	// archives the view, starts the replicant process
	virtual	status_t	Archive(BMessage *data, bool deep = true) const;
	// Draws the View, system calls it whenever drawing is needed
	virtual	void	Draw(BRect updateRect);
	// program's standard receive message thing
	virtual	void	MessageReceived(BMessage *msg);
	// called once a second to increment the clock
	virtual void	Pulse();
	// takes the mouse down event
	virtual void	MouseDown(BPoint point);

	float			CalcViewWidth(void);	// calculates the View's width
	float			CalcViewHeight(void);	// calculates the View's height
	void			SetColor(int face);		// Sets the clock color
	void			SetTwentyFour(bool twentyfr, bool draw = true);	// Sets the clock in 12 or 24 hour mode
	int				numcolor;		// the number of the selected color.
									// saved between runs of the program
	bool			twentyfour;		// Whether the clock is in 24 hour mode
	bool 			mIsReplicant;	// whether the view is a replicant or not

private:
	typedef	BView inherited;		// The Be person did this

	short			fHours;			// The currest hour
	short			fMinutes;		// The current minute
	short			fSeconds;		// The current second
	int 			prevhour;		// used for efficiency
	int 			prevmin;		// used for efficiency
	int 			prevsec;		// used for efficiency
	BRect			fRect;			// The coordinate rectangle of the view
	bool			FirstDraw;		// Whether it is the first draw or not
	bool			DraggerMade;	// Whether the BDragger has been made
	int16			numcolors;		// the number of colors currently loaded

	BPoint			DotSize;		// The x and y values of the Dot's BBitmap
	BBitmap			*DotOn[COLOR_ARRAY_SIZE];	// contains all the different On graphics
	BBitmap			*DotOff[COLOR_ARRAY_SIZE];	// contains all the different Off graphics
	BBitmap 		*CurrOn; 		// the current clock dot when it is on
	BBitmap 		*CurrOff;		// the current clock dot when it is off

	TMainMenu		*TheMenu;		// The main popup menu for the view & replicant
};