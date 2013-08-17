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
TMainMenu::TMainMenu(char *name, BDirectory *graphicsDir, int tnumcolors) : BPopUpMenu(name, false, false, B_ITEMS_IN_COLUMN)
{
	BMenuItem *tmpItem;
	BMenuItem *DotColor[COLOR_ARRAY_SIZE];

	printf("-1-MainMenu\n");

	numcolors = tnumcolors;

	int i = 0;	// counting variable
	BDirectory *colorDir = new BDirectory();
	BEntry *nextColorDir = new BEntry();
	BPath *curPath = new BPath();
	BMessage *tmpMsg;
	char curDirStr[B_FILE_NAME_LENGTH];
	graphicsDir->Rewind();
	while (graphicsDir->GetNextEntry(nextColorDir) == B_OK) {
		colorDir = new BDirectory(nextColorDir);
		if ((colorDir->Contains("on", B_FILE_NODE)) && (colorDir->Contains("off", B_FILE_NODE))) {
			if (nextColorDir->GetPath(curPath) == B_OK) {
				sprintf(curDirStr, "%s", curPath->Leaf());
				tmpMsg = new BMessage(CHNGCOLOR);
				tmpMsg->AddInt16("newcolor", i);
				DotColor[i] = new BMenuItem(curDirStr, tmpMsg);
				i++;
			}
		}
	}

	if (i == numcolors) {
		for (int a=0; a<numcolors; a++) {
			AddItem(DotColor[a]);
		}

		AddSeparatorItem();
	
		tmpMsg = new BMessage(CHNG12HR);
		tmpItem = new BMenuItem("12 Hour", tmpMsg);
		AddItem(tmpItem);
		tmpMsg = new BMessage(CHNG24HR);
		tmpItem = new BMenuItem("24 Hour", tmpMsg);
		AddItem(tmpItem);
	
		printf("-3-MainMenu\n");
	}
	else {
		BAlert *ExitAlert = new BAlert("Fatal Error", "The contents of the \"[user settings]/BinaryClock_2.xx/Graphics/\" directory changed during execution.", "I'm Sorry", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
		ExitAlert->Go();
		exit(1);
	}

	DotColor[0]->SetMarked(true);
}

// CONSTRUCTOR from Archive
TMainMenu::TMainMenu(BMessage *archive) : BPopUpMenu(archive)
{
	BMessage btemp;

	// get number of colors archived
	numcolors = archive->FindInt16("numcolors");
	
	printf("-MainMenu-CONSTRUCTOR from Archive - numcolors: %i\n", numcolors);
}

// Archive function
status_t TMainMenu::Archive(BMessage *archive, bool deep) const
{
	if (BPopUpMenu::Archive(archive, deep) != B_OK) {
		return B_ERROR;
	}

	archive->AddInt16("numcolors", numcolors);

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
