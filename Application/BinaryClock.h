/****************************************************************
*				BinaryClock.h									*
*				Version 2.0										*
*				by David Enderson (david@enderson.net)			*
*---------------------------------------------------------------*
* This code is Copyright 2000 under the GPL.					*
* See the Readme file included in this directory for more info.	*
****************************************************************/
#ifndef _APPLICATION_H
#include <Application.h>
#endif

#include "BinaryClockWindow.h"

class TBinaryClock : public BApplication {

public:
					TBinaryClock();
	virtual	void	MessageReceived(BMessage *msg);

private:
	TBCWindow		*myWindow;
	TBCView			*myView;
};
