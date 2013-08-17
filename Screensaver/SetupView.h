/****************************************************************************************************
*	SetupView.h																						*
*																									*
*	Version:	0.7 																				*
*	Author: 	David Enderson																		*
*	E-mail:		neelix@festing.org																	*
*	Webpage:	http://neelix.festing.org															*
*	This source code is released under the GPL														*
*---------------------------------------------------------------------------------------------------*
* See the rest of the comments in BinaryClock.cpp													*
*																									*
* My biggest thanks goes to Stefan Arentz (stefan.arentz@soze.com) for the use of his SetupView		*
* files.  I adapted them to fit my needs.															*
****************************************************************************************************/

#ifndef __SETUPVIEW_H__
#define __SETUPVIEW_H__

#include "View.h"
#include "RadioButton.h"

#define maxstars 501

//---------------------------------------------------------------------------------------------------

class SetupView : public BView
{
	public:
		SetupView(BRect frame, const char *name, int32* ioColor);
		void AttachedToWindow();
		void MessageReceived(BMessage *msg);
		
	private:
		int32* mColor;			// color of clock to be passed
		BView *rbview;			// the view the radio buttons go on
		BRadioButton *rbCyan;	// the cyan radio button
		BRadioButton *rbPurple;	// the purple radio button
		BRadioButton *rbRed;	// the red radio button
};

//---------------------------------------------------------------------------------------------------

#endif
