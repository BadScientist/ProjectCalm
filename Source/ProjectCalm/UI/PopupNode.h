#pragma once

#include "PopupWidget.h"


struct FPopupNode
{
	FPopupNode(){};

	UPopupWidget* Popup{nullptr};
	FPopupNode* Next{nullptr};

};
