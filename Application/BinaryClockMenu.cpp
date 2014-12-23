/****************************************************************
*				BinaryClockMenu.cpp								*
*				Version 2.0										*
*				by David Enderson (david@enderson.net)			*
*---------------------------------------------------------------*
* This code is Copyright 2000 under the GPL.					*
* See the Readme file included in this directory for more info.	*
****************************************************************/
#include <stdio.h>
#include <Alert.h>
#include <Directory.h>
#include <Entry.h>
#include <Path.h>

#include "BinaryClockMenu.h"

// Constructor
TMainMenu::TMainMenu(char *name, const char* colorNames[]) : BPopUpMenu(name, false, false, B_ITEMS_IN_COLUMN)
{
	BMenuItem *tmpItem;

	printf("-1-MainMenu\n");

	for (int i = 0; colorNames[i] != NULL; i++) {
		BMessage* msg = new BMessage(CHNGCOLOR);
		msg->AddInt16("newcolor", i);
		BMenuItem* item = new BMenuItem(colorNames[i], msg);
		AddItem(item);
		if (i == 0)
			item->SetMarked(true);
	}

	AddSeparatorItem();

	tmpItem = new BMenuItem("12 Hour", new BMessage(CHNG12HR));
	AddItem(tmpItem);
	tmpItem = new BMenuItem("24 Hour", new BMessage(CHNG24HR));
	AddItem(tmpItem);

	printf("-3-MainMenu\n");
}

// CONSTRUCTOR from Archive
TMainMenu::TMainMenu(BMessage *archive) : BPopUpMenu(archive)
{
}

// Archive function
status_t TMainMenu::Archive(BMessage *archive, bool deep) const
{
	if (BPopUpMenu::Archive(archive, deep) != B_OK) {
		return B_ERROR;
	}

	return B_OK;
}

//-----------------------------------------------------------------

BArchivable *TMainMenu::Instantiate(BMessage *archive)
{
	if (validate_instantiation(archive, "TMainMenu"))
		return new TMainMenu(archive);
	return NULL;
}

//-----------------------------------------------------------------

// Destructor for the Menu
TMainMenu::~TMainMenu()
{
	// this is empty for now
}
