/****************************************************************
*				BinaryClockMenu.h								*
*				Version 2.0										*
*				by David Enderson (david@enderson.net)			*
*---------------------------------------------------------------*
* This code is Copyright 2000 under the GPL.					*
* See the Readme file included in this directory for more info.	*
****************************************************************/
#include <MenuItem.h>
#include <PopUpMenu.h>

#include "messages.h"

#define COLOR_ARRAY_SIZE 100

class _EXPORT TMainMenu : public BPopUpMenu {

public:
						TMainMenu(char *name, BDirectory *graphicsDir, int tnumcolors); 
	// revives the menu from being archived 
						TMainMenu(BMessage *archive);
	// receives the revive call and calls the message only constructor
	static	BArchivable	*Instantiate(BMessage *archive);
	// archives the view, starts the replicant process
	virtual	status_t	Archive(BMessage *archive, bool deep = true) const;
	virtual				~TMainMenu();

private:	
	int		numcolors;
};
