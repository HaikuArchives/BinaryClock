/****************************************************************
*				BinaryClockView.cpp								*
*				Version 2.0										*
*				by David Enderson (david@enderson.net)			*
*---------------------------------------------------------------*
* This code is Copyright 2000 under the GPL.					*
* See the Readme file included in this directory for more info.	*
****************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <float.h>
#include <math.h>

#include <Alert.h>
#include <Application.h>
#include <Debug.h>
#include <Directory.h>
#include <Dragger.h>
#include <Entry.h>
#include <FindDirectory.h>
#include <MenuItem.h>
#include <Path.h>
#include <Resources.h>
#include <TranslationKit.h>

#include "BinaryClockView.h"

// ----------------------------------------------------------------
// Constructor for the View on the screen
TBCView::TBCView(BRect viewRect, char *title, int face, bool twentyfr)
	:BView(viewRect, title, B_NOT_RESIZABLE, B_WILL_DRAW | B_DRAW_ON_CHILDREN | B_PULSE_NEEDED)
{
	int i;				// temp loop counter - reused
	//status_t success;	// reused many times to check status of an operation
	bool success = true;
	
	mIsReplicant = false;
	FirstDraw = true;
	DraggerMade = false;
	
	// Sets the background color as Black
	SetViewColor(0,0,0);

	// temporary rectangle for constructing CurrOn and CurrOff
	BRect	theRect;
	theRect.Set(0,0,11,11);

	// setup all the light images
	i = 0;
	BDirectory *graphicsDir = new BDirectory();
	BPath *graphicsPath = new BPath();

	if (find_directory(B_USER_SETTINGS_DIRECTORY, graphicsPath) == B_OK) {
		graphicsPath->Append("BinaryClock_2.xx/Graphics");
		if (graphicsDir->SetTo(graphicsPath->Path()) == B_OK) {
			// Graphics directory exists
			BDirectory *colorDir = new BDirectory();
			BEntry *nextColorDir = new BEntry();
			BPath *curPath = new BPath();
			char curPathStr[B_PATH_NAME_LENGTH];
			graphicsDir->Rewind();
			while (graphicsDir->GetNextEntry(nextColorDir) == B_OK) {
				colorDir = new BDirectory(nextColorDir);
				if ((colorDir->Contains("on", B_FILE_NODE)) && (colorDir->Contains("off", B_FILE_NODE))) {
					if (nextColorDir->GetPath(curPath) == B_OK) {
						if (i == COLOR_ARRAY_SIZE) {
							BAlert *ExitAlert = new BAlert("Fatal Error", "You have more choices in the Graphics directory than this program can handle.  Increase COLOR_ARRAY_SIZE in the source code and recompile.", "I'm Sorry", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
							ExitAlert->Go();
							exit(1);
						}
						sprintf(curPathStr, "%s/on", curPath->Path());
						DotOn[i] = BTranslationUtils::GetBitmapFile(curPathStr);
						sprintf(curPathStr, "%s/off", curPath->Path());
						DotOff[i] = BTranslationUtils::GetBitmapFile(curPathStr);
						i++;
					}
				}
			}
			numcolors = i;
	
			// This should not be added to the view as a child.
			TheMenu = new TMainMenu("themenu", graphicsDir, numcolors);
		}
		else {
			success = false;
		}	
	}
	else {
		success = false;
	}	

	// Graphics directory does not exist
	//   Appraise the user of the problem and exit
	if (!success) {
		char exitMsg[B_PATH_NAME_LENGTH];
		sprintf(exitMsg, "The \"%s\" directory does not exist.", graphicsPath->Path());
		BAlert *ExitAlert = new BAlert("Fatal Error", exitMsg, "I'm Sorry", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
		ExitAlert->Go();
		exit(1);
	}

	// sets the time
	short		hours,minutes,seconds;
	struct tm	*loctime;
	time_t		current;
	current = time(0);
	loctime = localtime(&current);
	hours = loctime->tm_hour;
	minutes = loctime->tm_min;
	seconds = loctime->tm_sec;
	fHours = hours;
	fMinutes = minutes;
	fSeconds = seconds;

	if (!twentyfour) {
		if (fHours > 12)
			fHours -= 12;
		if (fHours == 0)
			fHours = 12;
	}

	// sets them so it will draw the first time for sure
	prevmin = 777;
	prevhour = 777;

	// construct dragger
	BRect dr = this->Bounds();
	dr.bottom = dr.top+7;
	dr.right = dr.left+7;
	BDragger *Dragger = new BDragger(dr, this, 0);
	this->AddChild(Dragger);
	DraggerMade = true;

	SetTwentyFour(twentyfr, false);
	
	SetColor(face);
}

// ----------------------------------------------------------------

void TBCView::SetTwentyFour(bool twentyfr, bool draw = true)
{
	BMenuItem *tmpItem;

	twentyfour = twentyfr;
	if (twentyfour) {
		tmpItem = TheMenu->FindItem("24 Hour");
		tmpItem->SetMarked(true);
		tmpItem = TheMenu->FindItem("12 Hour");
		tmpItem->SetMarked(false);
	}
	else {
		tmpItem = TheMenu->FindItem("12 Hour");
		tmpItem->SetMarked(true);
		tmpItem = TheMenu->FindItem("24 Hour");
		tmpItem->SetMarked(false);
	}

	if (draw) {
		Draw(this->Bounds());
	}
}

// ----------------------------------------------------------------

float TBCView::CalcViewWidth(void)
{
	return (DotSize.x * 6) + (float)((int)(LG_GAP * DotSize.x) * 2) + (SM_GAP * 5) - 1;
}

// ----------------------------------------------------------------

float TBCView::CalcViewHeight(void)
{
	return (DotSize.y * 4) + (SM_GAP * 5) - 1;
}

// ----------------------------------------------------------------

void TBCView::SetColor(int face)
{
	BMenuItem *mi;
	BWindow *ParentWindow;

	// sets the last chosen item on the menu as not checked
	mi = TheMenu->ItemAt(numcolor);
	if (mi) {
		mi->SetMarked(false);
	}

	numcolor = face;
	if ((numcolor < 0) || (numcolor >= numcolors)) {
		numcolor = 0;
	}

	// sets the chosen item on the menu as checked
	mi = TheMenu->ItemAt(numcolor);
	if (mi) {
		mi->SetMarked(true);
	}

	CurrOn=DotOn[numcolor];
	CurrOff=DotOff[numcolor];
	// Gets the size of the current loaded graphics
	// Makes the assumption the On and Off graphics are the same size
	BRect tempRect = CurrOn->Bounds();
	DotSize.x = tempRect.right - tempRect.left + 1;
	DotSize.y = tempRect.bottom - tempRect.top + 1;

	// Resizes the view.  Resizes parent window if necessary
	this->ResizeTo(CalcViewWidth(), CalcViewHeight());
	fRect = this->Bounds();

	// Checks to make it is currently not a replicant
	if (!mIsReplicant) {
		ParentWindow = this->Window();
		// ParentWindow would be NULL if this instance is a replicant
		if (ParentWindow) {
			ParentWindow->ResizeTo(CalcViewWidth(), CalcViewHeight());
		}
	}

	Draw(this->Bounds());
}

//=================================================================
// Empty Destructor
//-----------------------------------------------------------------
TBCView::~TBCView()
{
}


//=================================================================
// Constructor for Replicant
// ----------------------------------------------------------------
// Please pardon the debug code.  This is a work in progress, and
// I don't want to bother taking it out just to put it back in later.
TBCView::TBCView(BMessage *archive) : BView(archive)
{
	char buf[10];
	int face;
	bool twentyfr;
	BArchivable *unarchived;

	mIsReplicant = true;
	FirstDraw = true;
	DraggerMade = false;

	// get number of colors archived
	numcolors = archive->FindInt16("numcolors");
	
	// gets all the on and off images from the archive
	for (int a=0; a<numcolors; a++) {
		BMessage bon;
		BMessage bof;
		sprintf(buf, "%2don", a);
		archive->FindMessage(buf, &bon);
		unarchived = instantiate_object(&bon);
		DotOn[a] = dynamic_cast<BBitmap*>(unarchived);
		sprintf(buf, "%2dof", a);
		archive->FindMessage(buf, &bof);
		unarchived = instantiate_object(&bof);
		DotOff[a] = dynamic_cast<BBitmap*>(unarchived);
	}

	// This should not be added to the view as a child.
	BMessage bmenu;
	archive->FindMessage("themenu", &bmenu);
	unarchived = instantiate_object(&bmenu);
	TheMenu = dynamic_cast<TMainMenu*>(unarchived);

	// temporary rectangle for constructing CurrOn and CurrOff
	BRect	theRect;
	theRect.Set(0,0,11,11);

	// sets them so it will draw the first time for sure
	prevmin = 777;
	prevhour = 777;

	fRect = archive->FindRect("bounds");
	twentyfr = archive->FindBool("twentyfour");
	face = archive->FindInt16("face");

	// sets the time
	short		hours,minutes,seconds;
	struct tm	*loctime;
	time_t		current;
	current = time(0);
	loctime = localtime(&current);
	hours = loctime->tm_hour;
	minutes = loctime->tm_min;
	seconds = loctime->tm_sec;
	fHours = hours;
	fMinutes = minutes;
	fSeconds = seconds;

	SetTwentyFour(twentyfr, false);

	if (!twentyfour) {
		if (fHours > 12)
			fHours -= 12;
		if (fHours == 0)
			fHours = 12;
	}

	SetColor(face);

	// There is this weird deal with BDraggers.
	// See, when you add a dragger as a child to a view, and then implant that
	// view in the desktop, the dragger appears and works fine.  If you shut the
	// computer down the normal way with "Show Replicants" checked and reboot,
	// you'll see it next time.  But if you ever turn "Show Replicants" off and shutdown,
	// you never see the replicant handle again.  Then the only way to get rid of the
	// replicant is to delete Tracker's shelf.  This is true of Be's OWN Clock and
	// Pulse apps.  Yet the dragger pointer is still there, it just appears to be a bad
	// pointer.  So I just delete it and add another one in this constructor.  That seems
	// to be friendly to memory, avoid the duplicate problem, and let you ALWAYS have
	// access to the dragger.  Why Be didn't do this in their sample code I have no idea.
	BView *c = ChildAt(0);
	if (c) {
		RemoveChild(c);
	}

	// construct dragger
	BRect dr = this->Bounds();
	dr.bottom = dr.top+7;
	dr.right = dr.left+7;
	BDragger *Dragger = new BDragger(dr, this, 0);
	this->AddChild(Dragger);
	DraggerMade = true;
}

// ----------------------------------------------------------------

status_t TBCView::Archive(BMessage *archive, bool deep) const
{
	if (BView::Archive(archive, deep) != B_OK)
		return B_ERROR;

	archive->AddString("add_on", APP_SIGNATURE);
	archive->AddString("class", "BinaryClock");

	// got the next four lines of code from
	//   MonoView 0.3.1 source by Okada Jun
	//----------------------------------------
	// It's a quick hack to avoid BView's bug.
	BPoint origin;
	origin = archive->FindPoint("_origin");
	archive->RemoveName("_origin");
	archive->AddPoint("origin", origin);

	archive->AddRect("bounds", fRect);
	archive->AddInt16("face", numcolor);
	archive->AddInt16("numcolors", numcolors);
	archive->AddBool("twentyfour", twentyfour);

	char hap[10];

	// This is true if all the children are being archived also
	if (deep) {
		for (int a=0; a<numcolors; a++) {
			BMessage bon;
			BMessage bof;
			sprintf(hap, "%2don", a);
			if (DotOn[a]->Archive(&bon, deep) == B_OK) {
				archive->AddMessage(hap, &bon);
			}
			sprintf(hap, "%2dof", a);
			if (DotOff[a]->Archive(&bof, deep) == B_OK) {
				archive->AddMessage(hap, &bof);
			}
		}

		// archive the PopupMenu
		BMessage bmenu;
		if (TheMenu->Archive(&bmenu, deep) == B_OK) {
			archive->AddMessage("themenu", &bmenu);
		}
	}
	
	return B_OK;
}

// ----------------------------------------------------------------

BArchivable *TBCView::Instantiate(BMessage *data)
{
	if (!validate_instantiation(data, "BinaryClock"))
		return NULL;
	return new TBCView(data);
}

// ----------------------------------------------------------------

void TBCView::Pulse()
{
	// variables used to get the time
	short		hours,minutes,seconds;
	struct tm	*loctime;
	time_t		current;

	current = time(0);
	loctime = localtime(&current);
	hours = loctime->tm_hour;
	minutes = loctime->tm_min;
	seconds = loctime->tm_sec;

	fHours = hours;
	fMinutes = minutes;
	fSeconds = seconds;

	if (!twentyfour) {
		if (fHours > 12)
			fHours -= 12;
		if (fHours == 0)
			fHours = 12;
	}

	Draw(fRect);
}

// ----------------------------------------------------------------
/*
void TBCView::Draw(BRect rect)
{
	int a;		// temporary loop variable used many times in this function

	// fill the background with Black
	SetDrawingMode(B_OP_COPY);
	SetLowColor(0, 0, 0);
	FillRect(BRect(DotSize.x*2, 0.0, DotSize.x*2.5, DotSize.y*4), B_SOLID_LOW);
	FillRect(BRect(DotSize.x*4.5, 0.0, DotSize.x*5, DotSize.y*4), B_SOLID_LOW);

	// Draw Hours
	for (a=0; a<4; a++) {
		prevhour=fHours;
		if ((prevhour/10)&(int)pow(2,a))
			DrawBitmap(CurrOn,BPoint(0,(3-a)*DotSize.y));
		else
			DrawBitmap(CurrOff,BPoint(0,(3-a)*DotSize.y));
		if ((prevhour%10)&(int)pow(2,a))
			DrawBitmap(CurrOn,BPoint(DotSize.x,(3-a)*DotSize.y));
		else
			DrawBitmap(CurrOff,BPoint(DotSize.x,(3-a)*DotSize.y));
	}
	
	// Draw Minutes
	for (a=0; a<4; a++) {
		prevmin=fMinutes;
		if ((prevmin/10)&(int)pow(2,a))
			DrawBitmap(CurrOn,BPoint((DotSize.x*2)+(DotSize.x/2),(3-a)*DotSize.y));
		else
			DrawBitmap(CurrOff,BPoint((DotSize.x*2)+(DotSize.x/2),(3-a)*DotSize.y));
		if ((prevmin%10)&(int)pow(2,a))
			DrawBitmap(CurrOn,BPoint((DotSize.x*3)+(DotSize.x/2),(3-a)*DotSize.y));
		else
			DrawBitmap(CurrOff,BPoint((DotSize.x*3)+(DotSize.x/2),(3-a)*DotSize.y));
	}

	// Draw Seconds
	for (a=0; a<4; a++) {
		if ((fSeconds/10)&(int)pow(2,a))
			DrawBitmap(CurrOn,BPoint((DotSize.x*4)+(DotSize.x),(3-a)*DotSize.y));
		else
			DrawBitmap(CurrOff,BPoint((DotSize.x*4)+(DotSize.x),(3-a)*DotSize.y));
		if ((fSeconds%10)&(int)pow(2,a))
			DrawBitmap(CurrOn,BPoint((DotSize.x*5)+(DotSize.x),(3-a)*DotSize.y));
		else
			DrawBitmap(CurrOff,BPoint((DotSize.x*5)+(DotSize.x),(3-a)*DotSize.y));
	}
	
	// creates a handle to the dragger and redraws it I got these
	//   three lines from Be's Clock app source which came with the OS
	BView *c = ChildAt(0);
	if ((c) && (DraggerMade)) {
		c->Draw(c->Bounds());
	}
};
*/
void TBCView::Draw(BRect rect)
{
	int a;		// temporary loop variable used many times in this function
	float cury;	// temporary value used to centralize y calculations

	// fill the background with Black
	SetDrawingMode(B_OP_COPY);
	SetLowColor(0, 0, 0);

	//FillRect(BRect(0, 0, CalcViewWidth(), CalcViewHeight()), B_SOLID_LOW);

	// Draw Hours
	for (a=0; a<4; a++) {
		prevhour = fHours;
		cury = (3-a)*(DotSize.y+SM_GAP)+SM_GAP;
		// left hour column
		if ((prevhour/10)&(int)pow(2,a))
			DrawBitmap(CurrOn, BPoint(SM_GAP, cury));
		else
			DrawBitmap(CurrOff, BPoint(SM_GAP, cury));
		// right hour column
		if ((prevhour%10)&(int)pow(2,a))
			DrawBitmap(CurrOn, BPoint(DotSize.x+(SM_GAP*2), cury));
		else
			DrawBitmap(CurrOff, BPoint(DotSize.x+(SM_GAP*2), cury));
	}
	
	// creates a handle to the dragger and redraws it I got these
	//   three lines from Be's Clock app source which came with the OS
	BView *c = ChildAt(0);
	if ((c) && (DraggerMade)) {
		c->Draw(c->Bounds());
	}
	
	// Draw Minutes
	for (a=0; a<4; a++) {
		prevmin=fMinutes;
		cury = (3-a)*(DotSize.y+SM_GAP)+SM_GAP;
		// left minute column
		if ((prevmin/10)&(int)pow(2,a))
			DrawBitmap(CurrOn, BPoint(((DotSize.x+SM_GAP)*2)+(float)((int)(DotSize.x*LG_GAP)), cury));
		else
			DrawBitmap(CurrOff, BPoint(((DotSize.x+SM_GAP)*2)+(float)((int)(DotSize.x*LG_GAP)), cury));
		// right minute column
		if ((prevmin%10)&(int)pow(2,a))
			DrawBitmap(CurrOn,BPoint(((DotSize.x+SM_GAP)*3)+(float)((int)(DotSize.x*LG_GAP)), cury));
		else
			DrawBitmap(CurrOff,BPoint(((DotSize.x+SM_GAP)*3)+(float)((int)(DotSize.x*LG_GAP)), cury));
	}

	// Draw Seconds
	for (a=0; a<4; a++) {
		prevsec = fSeconds;
		cury = (3-a)*(DotSize.y+SM_GAP)+SM_GAP;
		// left second column
		if ((prevsec/10)&(int)pow(2,a))
			DrawBitmap(CurrOn, BPoint((DotSize.x*4)+(SM_GAP*3)+(float)((int)(DotSize.x*LG_GAP)*2), cury));
		else
			DrawBitmap(CurrOff, BPoint((DotSize.x*4)+(SM_GAP*3)+(float)((int)(DotSize.x*LG_GAP)*2), cury));
		// right second column
		if ((prevsec%10)&(int)pow(2,a))
			DrawBitmap(CurrOn, BPoint((DotSize.x*5)+(SM_GAP*4)+(float)((int)(DotSize.x*LG_GAP)*2), cury));
		else
			DrawBitmap(CurrOff, BPoint((DotSize.x*5)+(SM_GAP*4)+(float)((int)(DotSize.x*LG_GAP)*2), cury));
	}
};

// ----------------------------------------------------------------

void TBCView::MouseDown(BPoint point)
{
	BMenuItem *selected; 

	ConvertToScreen(&point); 
	selected = TheMenu->Go(point); 

	// "selected" returns NULL sometimes.  If you want to know
	//   why, check Be's docs on PopUpMenu's Go function. 
	//   Anyway, here I'm checking for that problem.
	if (selected) {
		int16 newcolor;
		switch ( selected->Message()->what ) {
			case CHNG12HR:
				SetTwentyFour(false);
				break;
			case CHNG24HR:
				SetTwentyFour(true);
				break;
			case CHNGCOLOR:
				newcolor = selected->Message()->FindInt16("newcolor");
				SetColor(newcolor);
				break;
			default:
				break;
		}
	}
}

// ----------------------------------------------------------------

void TBCView::MessageReceived(BMessage *msg)
{
	switch(msg->what) {
		case B_ABOUT_REQUESTED:
			(new BAlert("About BinaryClock", "BinaryClock\nby David Enderson\nVersion 2.0\n\nemail: david@enderson.net\nwebpage: http://david.enderson.net","That's Nice"))->Go();
			break;
		default:
			inherited::MessageReceived(msg);
	}
}
