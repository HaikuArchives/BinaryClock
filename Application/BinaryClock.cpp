/****************************************************************
*				BinaryClock.cpp									*
*				Version 2.0										*
*				by David Enderson (david@enderson.net)			*
*---------------------------------------------------------------*
* This code is Copyright 2000 under the GPL.					*
* See the Readme file included in this directory for more info.	*
****************************************************************/
#include <Debug.h>
#include <Path.h>
#include <Screen.h>
#include <FindDirectory.h>
#include <TranslationKit.h>

#include "BinaryClock.h"

int main(int argc, char* argv[])
{	
	TBinaryClock *myApp;

	myApp = new TBinaryClock();
	myApp->Run();

	delete myApp;
	return 0;
}

TBinaryClock::TBinaryClock() :BApplication(APP_SIGNATURE)
{
	BRect		windowRect,	// Rectange the Window will be put at
				viewRect;	// Rectangle the View will be put at
	BPoint		wind_loc;	// Location of the top left point of the window
	BRect		posRect;	// Last saved BRect position of window
	int			ref;		// some reference thingie or other
							// Don't look at me.  The person at Be did this.
	int			face;		// 0 based number denoting the current color of the view.
							// Red is the default, thus it is 3 until otherwise changed.
	bool		twentyfr;	// Whether the program is in 24 hour mode
	BPath		path;		// Disk path to where the app settings were hopefully saved.
	BBitmap*	tempBitmap = NULL; // Temporary Bitmap* for sizing Window correctly

	// sets default face number incase settings file isn't found
	face = 3;
	
	bool settingsFound = false;
	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path) == B_OK) {
		path.Append("BinaryClock_2.xx/settings");
		ref = open(path.Path(), O_RDONLY);
		if (ref >= 0) {
			read(ref, (char *)&wind_loc, sizeof(wind_loc));
			read(ref, (char *)&face, sizeof(int));
			read(ref, (char *)&posRect, sizeof(BRect));
			read(ref, (char *)&twentyfr, sizeof(bool));
			close(ref);
			myWindow->MoveTo(wind_loc);
			settingsFound = true;
		}
	}
	
	if (tempBitmap) {
		BRect tempRect = tempBitmap->Bounds();
		viewRect.Set(0, 0, (tempRect.right-tempRect.left+1)*7-1, (tempRect.bottom-tempRect.top+1)*4-1);
		windowRect.Set(wind_loc.x, wind_loc.y, (tempRect.right-tempRect.left+1)*7+wind_loc.x-1, (tempRect.bottom-tempRect.top+1)*4+wind_loc.y-1);
	}
	else {
		viewRect.Set(0, 0, posRect.right-posRect.left, posRect.bottom-posRect.top);
		windowRect = posRect;
	}

	myWindow = new TBCWindow(windowRect, "BinaryClock");
	myView = new TBCView(viewRect, "BinaryClock", face, twentyfr);

	myWindow->Lock();
	myWindow->AddChild(myView);
	myWindow->TheView = myView;

	// Checks to see if the window is outside the
	// screen and if so moves it to 100, 100.
	BRect frame = myWindow->Frame();
	frame.InsetBy(-4, -4);
	if (!frame.Intersects(BScreen(myWindow).Frame()) || !settingsFound) {
		// it's not visible so reposition. I'm not going to get
		// fancy here, just place in the default location
		myWindow->CenterOnScreen();
	}

	myWindow->ResizeTo(myView->CalcViewWidth(), myView->CalcViewHeight());

	myWindow->Show();
	myWindow->Unlock();  // I'm unsure if this is necessary
}

// Even though this looks standard, it still needs to be coded.
//   Otherwise, the messages aren't passed to the view.
//   I learned this the hard way.  :-/
void TBinaryClock::MessageReceived(BMessage *msg)
{
	BApplication::MessageReceived(msg);
}
