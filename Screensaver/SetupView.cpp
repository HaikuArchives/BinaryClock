/****************************************************************************************************
*	SetupView.cpp																					*
*																									*
*	Version:	0.7																					*
*	Author: 	David Enderson																		*
*	E-mail:		neelix@festing.org																	*
*	Webpage:	http://neelix.festing.org															*
*	This source code is released under the GPL														*
*---------------------------------------------------------------------------------------------------*
* See the rest of the comments in BinaryClock.cpp													*
*																									*
* My biggest thanks goes to Stefan Arentz (stefan.arentz@soze.com).  I couldn't figure out how to	*
* get my slider bars to work correctly, so I used his SetupView.cpp and SetupView.h files and then	*
* changed them to fit my needs.																		*
****************************************************************************************************/

#include <stdio.h>
#include <be/support/SupportDefs.h>
#include <be/interface/StringView.h>
#include <be/interface/RadioButton.h>
#include <be/interface/View.h>
#include <be/interface/Font.h>
#include "SetupView.h"

//---------------------------------------------------------------------------------------------------

SetupView::SetupView(BRect frame, const char *name, int32* ioColor)
	:	BView(frame, name, 0, B_FOLLOW_ALL), mColor(ioColor)
{

}

//---------------------------------------------------------------------------------------------------

void SetupView::AttachedToWindow()
{
	// Use the same background color as the screensaver does
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	// Adds text to appear in the screensaver window	
	this->AddChild(new BStringView(BRect(10, 0, 240, 15), NULL, "BinaryClock: version 0.7, by David Enderson"));
	this->AddChild(new BStringView(BRect(10, 16, 240, 31), NULL, "email:   neelix@festing.org  "));
	this->AddChild(new BStringView(BRect(10, 32, 240, 47), NULL, "webpage: http://neelix.festing.org"));
	this->AddChild(new BStringView(BRect(10, 115, 240, 130), NULL, "Sorry the colors on the preview don't match"));
	this->AddChild(new BStringView(BRect(10, 131, 240, 146), NULL, "perfectly with the saver's real colors."));
	this->AddChild(new BStringView(BRect(10, 147, 240, 162), NULL, "This will be fixed in the next version."));

	rbview = new BView(BRect(10, 50, 160, 108), "", B_FOLLOW_NONE, B_WILL_DRAW);
	rbview->SetViewColor(215,215,215);
	this->AddChild(rbview);

	rbCyan = new BRadioButton(BRect(10,1,60,19), "", "Cyan", new BMessage('cyan'));
	rbCyan->SetTarget(this);
	rbview->AddChild(rbCyan);
	rbPurple = new BRadioButton(BRect(10,22,60,41), "", "Purple", new BMessage('purp'));
	rbPurple->SetTarget(this);
	rbview->AddChild(rbPurple);
	rbRed = new BRadioButton(BRect(10,43,60,63), "", "Red", new BMessage('red'));
	rbRed->SetTarget(this);
	rbview->AddChild(rbRed);

	if ((*mColor)==1)
		rbCyan->SetValue(*mColor);
	if ((*mColor)==2)
		rbPurple->SetValue(*mColor);
	if ((*mColor)==3)
		rbRed->SetValue(*mColor);
}

//---------------------------------------------------------------------------------------------------

void SetupView::MessageReceived(BMessage *msg)
{	
	switch(msg->what)
	{
		case 'cyan':
			*mColor = 1;
			break;
		case 'purp':
			*mColor = 2;
			break;
		case 'red':
			*mColor = 3;
			break;
		default :
			BView::MessageReceived(msg);
			break;
	}
}