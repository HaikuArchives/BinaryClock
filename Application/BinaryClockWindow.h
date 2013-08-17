/****************************************************************
*				BinaryClockWindow.h								*
*				Version 2.0										*
*				by David Enderson (david@enderson.net)			*
*---------------------------------------------------------------*
* This code is Copyright 2000 under the GPL.					*
* See the Readme file included in this directory for more info.	*
****************************************************************/
#ifndef _WINDOW_H
#include <Window.h>
#endif

#include "BinaryClockView.h"

class TBCWindow : public BWindow {

public:
					TBCWindow(BRect, const char*);
	virtual	bool	QuitRequested( void );

	TBCView			*TheView;
};
