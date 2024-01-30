#pragma once

#include "PopupMenu.h"


struct FPopupMenuNode
{
	FPopupMenuNode(){};

	UPopupMenu* Menu{nullptr};
	FPopupMenuNode* Next{nullptr};

};
