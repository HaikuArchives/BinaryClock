/****************************************************************
*	BinaryClockSS.cpp											*
*																*
*	Version:	0.7												*
*	Author: 	David Enderson									*
*	E-mail:		neelix@festing.org								*
*	Webpage:	http://neelix.festing.org						*
*	this source code is released under the GPL					*
*---------------------------------------------------------------*
* I was inspired to write this by my BinaryClock program.		*
* I wrote this with immense help from Urban Lindeskog			*
* (ubbe@rit.se).  I used his code for MySpots as the skeleton	*
* for this.  He in turn used the Be example code located at:	*
* /optional/sample-code/screen_savers/Spots						*
* Future plans for BinaryClock:									*
*	make it much more efficient									*
*	add other features I haven't thought of						*
* If you have advice on any of this or adapt this program in	*
* any way, PLEASE email me the code.  It would help me greatly!	*
*---------------------------------------------------------------*
* How I did the graphics:										*
* I made the 24bit RAW format non-interlaced graphics using     *
* Paint Shop Pro for Windows 95/98.  Then I ran the terminal	*
* command: craw width height filename							*
* example: craw 150 150 hgblue2.raw								*
* This command converts raw files as described above into an 	*
* array of unsigned characters and puts them automatically into *
* a file called: "iconfile".  I then include this file and make *
* this call in the program to include the raw file:				*
* 	CurrOn = new BBitmap(theRect, B_RGB32);						*
*	CurrOn->SetBits(hgblue2on,(150*150*3),0,B_RGB32);			*
*																*
* This is covered in more detail at:							*
* http://www-classic.be.com/aboutbe/benewsletter/Issue101.html  *
****************************************************************/
#include "BinaryClockSS.h"
#include <StringView.h>
#include <stdlib.h>
#include "redpict"
#include "purppict"
#include "cyanpict"
#include "smdots"

extern "C" _EXPORT BScreenSaver *instantiate_screen_saver(BMessage *message, image_id image)
{
	return new BinaryClock(message, image);
}

BinaryClock::BinaryClock(BMessage *message, image_id image)
	:
	BScreenSaver(message, image),
	CurrOn(NULL),
	CurrOff(NULL)
{
	// retrieves number of stars
	if (message->FindInt32("mcolor", &mcolor) != B_OK) {
		mcolor = 2;
	}
}

status_t BinaryClock::SaveState(BMessage *into) const
{
	// saves the color of the clock
	into->AddInt32("mcolor", mcolor);
	return B_OK;
}

void BinaryClock::StartConfig(BView *view)
{
	configview = new SetupView(view->Bounds(), "setup", &mcolor);
	view->AddChild(configview);

	// initializes CurrOn and CurrOff so I can delete them always at the beginning of StartSaver
	BRect theRect;								// temporary rectangle for constructing CurrOn and CurrOff
	theRect.Set(0,0,12,12);
	CurrOn = new BBitmap(theRect, B_RGB32);
	CurrOff = new BBitmap(theRect, B_RGB32);
}

status_t BinaryClock::StartSaver(BView *view, bool preview)
{
	// have to delete them before I initialize them below
	delete CurrOn;
	delete CurrOff;

	// sets the last time run to 0 so the display will be updated immediately
	oldsec = 0;

	// initializes global variables of screen width and height
	width=(view->Bounds()).IntegerWidth();		// width of screen
	height=(view->Bounds()).IntegerHeight();	// height of screen
	// temporary variables for this function
	rgb_color lowcolor = {0, 0, 0, 0};			// screen color
	int gwidth, gheight;						// the width & height of a graphic
	BRect theRect;								// temporary rectangle for constructing CurrOn and CurrOff
	int colwidth;								// width in pixels between columns
	int bgcolwidth;								// width of the two main column gaps in the clock
	
	if (width<635) {
		// I hope to make these 15x15 someday with the exact colors
		gwidth=12;
		gheight=12;
		colwidth = 2;
		bgcolwidth = 8;
		theRect.Set(0,0,(gwidth-1),(gheight-1));
		CurrOn = new BBitmap(theRect, B_RGB32);
		CurrOff = new BBitmap(theRect, B_RGB32);
		if (mcolor==1) {
			CurrOn->SetBits(cyanon,864,0,B_RGB32);
			CurrOff->SetBits(cyanoff,864,0,B_RGB32);
		}
		else if (mcolor==2) {
			CurrOn->SetBits(purpon,864,0,B_RGB32);
			CurrOff->SetBits(purpoff,864,0,B_RGB32);	
		}		
		else {
			CurrOn->SetBits(redon,864,0,B_RGB32);
			CurrOff->SetBits(redoff,864,0,B_RGB32);	
		}		
	}
	else
	if (width<1000) {
		gwidth=100;
		gheight=100;
		colwidth = 2;
		bgcolwidth = 13;
		theRect.Set(0,0,(gwidth-1),(gheight-1));
		CurrOn = new BBitmap(theRect, B_RGB32);
		CurrOff = new BBitmap(theRect, B_RGB32);
		if (mcolor==1) {
			CurrOn->SetBits(bgcyon,30000,0,B_RGB32);
			CurrOff->SetBits(bgcyof,30000,0,B_RGB32);
		}
		else if (mcolor==2) {
			CurrOn->SetBits(bgpron,30000,0,B_RGB32);
			CurrOff->SetBits(bgprof,30000,0,B_RGB32);
		}
		else {
			CurrOn->SetBits(bgrdon,30000,0,B_RGB32);
			CurrOff->SetBits(bgrdof,30000,0,B_RGB32);
		}
		
	}
	else {
		gwidth=150;
		gheight=150;
		colwidth = 1;
		bgcolwidth = 40;
		theRect.Set(0,0,(gwidth-1),(gheight-1));
		CurrOn = new BBitmap(theRect, B_RGB32);
		CurrOff = new BBitmap(theRect, B_RGB32);
		if (mcolor==1) {
			CurrOn->SetBits(hgcyon,67500,0,B_RGB32);
			CurrOff->SetBits(hgcyof,67500,0,B_RGB32);
		}
		else if (mcolor==2) {
			CurrOn->SetBits(hgpron,67500,0,B_RGB32);
			CurrOff->SetBits(hgprof,67500,0,B_RGB32);
		}
		else {
			CurrOn->SetBits(hgrdon,67500,0,B_RGB32);
			CurrOff->SetBits(hgrdof,67500,0,B_RGB32);
		}
	};

	// left and top boundary of drawing, used to cut down on processing time later in the drawpoint loop
	int leftstart = (width-((gwidth*6)+(colwidth*3)+(bgcolwidth*2)))/2;
	int topstart = (height-((gheight*4)+(colwidth*3)))/2;
	
	for (int a=0; a<24; a++)
		drawpoint[a].Set(((a/4)*(gwidth+colwidth))+((a/8)*bgcolwidth)+leftstart, ((3-(a%4))*(gheight+colwidth))+topstart);

	view->SetLowColor(lowcolor); 
	view->FillRect( view->Bounds(), B_SOLID_LOW ); 		
	view->SetViewColor(lowcolor);
	view->Invalidate();

	oldmcolor = mcolor;

	SetTickSize(50000);

	return B_OK;
}

void BinaryClock::Draw(BView *view, int32 frame)
{
	int a;			// temporary loop variable.  I figured it was more efficient to define it once here

	// used to get the time
	time_t clock = time( (time_t*) NULL); 
	struct tm *tmptr = localtime(&clock);

	if (tmptr->tm_sec!=oldsec) {
		// these may be moved global later to further optimize
		int prevhour;
		int prevmin;
		int prevsec;

		oldsec = tmptr->tm_sec;
		for (a=0; a<4; a++) {
			prevhour=tmptr->tm_hour;
			if ((prevhour/10)&(int)pow(2,a))
				view->DrawBitmap(CurrOn,drawpoint[a]);
			else
				view->DrawBitmap(CurrOff,drawpoint[a]);
		}
		for (a=4; a<8; a++) {
			prevhour=tmptr->tm_hour;
			if ((prevhour%10)&(int)pow(2,a-4))
				view->DrawBitmap(CurrOn,drawpoint[a]);
			else
				view->DrawBitmap(CurrOff,drawpoint[a]);
		}
		for (a=8; a<12; a++) {
			prevmin=tmptr->tm_min;
			if ((prevmin/10)&(int)pow(2,a-8))
				view->DrawBitmap(CurrOn,drawpoint[a]);
			else
				view->DrawBitmap(CurrOff,drawpoint[a]);
		}
		for (a=12; a<16; a++) {
			prevmin=tmptr->tm_min;
			if ((prevmin%10)&(int)pow(2,a-12))
				view->DrawBitmap(CurrOn,drawpoint[a]);
			else
				view->DrawBitmap(CurrOff,drawpoint[a]);
		}
		for (a=16; a<20; a++) {
			prevsec=tmptr->tm_sec;
			if ((prevsec/10)&(int)pow(2,a-16))
				view->DrawBitmap(CurrOn,drawpoint[a]);
			else
				view->DrawBitmap(CurrOff,drawpoint[a]);
		}
		for (a=20; a<24; a++) {
			prevsec=tmptr->tm_sec;
			if ((prevsec%10)&(int)pow(2,a-20))
				view->DrawBitmap(CurrOn,drawpoint[a]);
			else
				view->DrawBitmap(CurrOff,drawpoint[a]);
		}
	}

	if (mcolor!=oldmcolor)
		StartSaver(view, TRUE);	
}
